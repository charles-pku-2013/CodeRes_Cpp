#!/usr/bin/env python3
"""Flatten a Docker image to one filesystem layer and migrate image metadata.

The script streams `docker export` directly into `docker import`, so it does
not need a second image-sized tar file on disk. Runtime/image configuration is
read from `docker image inspect`, reapplied in a metadata-only build, and
verified before the requested target tag is created.

python3 flatten_docker_image.py --sudo --force 原镜像 新镜像
"""

from __future__ import annotations

import argparse
import json
import os
import shlex
import subprocess
import sys
import tempfile
import uuid
from pathlib import Path
from typing import Any, Iterable


SCRIPT_VERSION = "1.2.1"

MANAGED_CONFIG_KEYS = {
    "Cmd",
    "Entrypoint",
    "Env",
    "ExposedPorts",
    "Healthcheck",
    "Labels",
    "OnBuild",
    "Shell",
    "StopSignal",
    "User",
    "Volumes",
    "WorkingDir",
}


def eprint(*args: Any) -> None:
    print(*args, file=sys.stderr, flush=True)


def dockerfile_json(value: Any) -> str:
    return json.dumps(value, ensure_ascii=False, separators=(",", ":"))


def dockerfile_word(value: str) -> str:
    # Dockerfile's parser accepts a JSON-style double-quoted word. This keeps
    # spaces, quotes and backslashes in ENV/LABEL values from becoming tokens.
    return json.dumps(value, ensure_ascii=False)


def dockerfile_key_value(key: str, value: str) -> str:
    # Quote key and value separately. Besides matching Dockerfile's documented
    # LABEL/ENV form, this avoids a compatibility bug in some Docker Engine
    # versions when an entire "key=value with spaces" pair is one quoted word.
    return f"{dockerfile_word(key)}={dockerfile_word(value)}"


def human_size(size: int) -> str:
    value = float(size)
    for unit in ("B", "KiB", "MiB", "GiB", "TiB"):
        if value < 1024 or unit == "TiB":
            return f"{value:.1f} {unit}"
        value /= 1024
    return f"{value:.1f} TiB"


class Docker:
    def __init__(self, use_sudo: bool, executable: str) -> None:
        self.prefix = (["sudo"] if use_sudo else []) + [executable]

    def display(self, args: Iterable[str]) -> str:
        return shlex.join(self.prefix + list(args))

    def run(
        self,
        args: list[str],
        *,
        capture: bool = False,
        check: bool = True,
        quiet_error: bool = False,
        env: dict[str, str] | None = None,
    ) -> subprocess.CompletedProcess[str]:
        completed = subprocess.run(
            self.prefix + args,
            text=True,
            stdout=subprocess.PIPE if capture else None,
            stderr=subprocess.PIPE if quiet_error else None,
            check=False,
            env=env,
        )
        if check and completed.returncode != 0:
            if quiet_error and completed.stderr:
                eprint(completed.stderr.rstrip())
            raise RuntimeError(
                f"命令执行失败（退出码 {completed.returncode}）：{self.display(args)}"
            )
        return completed

    def inspect(self, image: str, *, required: bool = True) -> dict[str, Any] | None:
        completed = self.run(
            ["image", "inspect", image],
            capture=True,
            check=False,
            quiet_error=True,
        )
        if completed.returncode != 0:
            if required:
                if completed.stderr:
                    eprint(completed.stderr.rstrip())
                raise RuntimeError(f"找不到或无法检查镜像：{image}")
            return None
        try:
            result = json.loads(completed.stdout)
            return result[0]
        except (json.JSONDecodeError, IndexError, TypeError) as exc:
            raise RuntimeError(f"无法解析镜像 {image} 的检查结果") from exc


def healthcheck_change(healthcheck: dict[str, Any] | None) -> str | None:
    if not healthcheck:
        return None

    test = healthcheck.get("Test") or []
    if not test:
        return None
    if test[0] == "NONE":
        return "HEALTHCHECK NONE"

    options: list[str] = []
    duration_options = (
        ("Interval", "interval"),
        ("Timeout", "timeout"),
        ("StartPeriod", "start-period"),
        ("StartInterval", "start-interval"),
    )
    for config_name, option_name in duration_options:
        nanoseconds = healthcheck.get(config_name)
        if nanoseconds:
            options.append(f"--{option_name}={int(nanoseconds)}ns")
    retries = healthcheck.get("Retries")
    if retries:
        options.append(f"--retries={int(retries)}")

    prefix = "HEALTHCHECK"
    if options:
        prefix += " " + " ".join(options)

    kind = test[0]
    if kind == "CMD":
        return f"{prefix} CMD {dockerfile_json(test[1:])}"
    if kind == "CMD-SHELL":
        command = test[1] if len(test) > 1 else ""
        return f"{prefix} CMD {command}"
    raise RuntimeError(f"不支持的 Healthcheck.Test 类型：{kind!r}")


def metadata_changes(
    config: dict[str, Any],
    include_onbuild: bool,
    include_workdir: bool = True,
) -> list[str]:
    changes: list[str] = []

    # Apply fields that support Dockerfile variable substitution before ENV.
    # This prevents a literal "$NAME" in a stored path/label from being
    # accidentally expanded using the final image environment.
    for key, value in (config.get("Labels") or {}).items():
        changes.append("LABEL " + dockerfile_key_value(key, value))

    for port in (config.get("ExposedPorts") or {}).keys():
        changes.append(f"EXPOSE {port}")

    if config.get("User"):
        changes.append("USER " + dockerfile_word(config["User"]))

    volumes = list((config.get("Volumes") or {}).keys())
    if volumes:
        changes.append("VOLUME " + dockerfile_json(volumes))

    if include_workdir and config.get("WorkingDir"):
        changes.append("WORKDIR " + dockerfile_word(config["WorkingDir"]))

    if config.get("StopSignal"):
        changes.append("STOPSIGNAL " + dockerfile_word(config["StopSignal"]))

    healthcheck = healthcheck_change(config.get("Healthcheck"))
    if healthcheck:
        changes.append(healthcheck)

    # ENTRYPOINT before CMD avoids Docker's normal ENTRYPOINT processing from
    # clearing an already-inherited CMD.
    if config.get("Entrypoint"):
        changes.append("ENTRYPOINT " + dockerfile_json(config["Entrypoint"]))
    if config.get("Cmd"):
        changes.append("CMD " + dockerfile_json(config["Cmd"]))

    if config.get("Shell"):
        changes.append("SHELL " + dockerfile_json(config["Shell"]))

    for item in config.get("Env") or []:
        key, separator, value = item.partition("=")
        if not separator:
            value = ""
        changes.append("ENV " + dockerfile_key_value(key, value))

    if include_onbuild:
        for trigger in config.get("OnBuild") or []:
            changes.append("ONBUILD " + trigger)

    return changes


def platform_of(info: dict[str, Any]) -> str:
    os_name = info.get("Os")
    architecture = info.get("Architecture")
    if not os_name or not architecture:
        raise RuntimeError("原镜像没有有效的 OS/Architecture 信息")
    platform = f"{os_name}/{architecture}"
    if info.get("Variant"):
        platform += f"/{info['Variant']}"
    return platform


def stream_export_import(
    docker: Docker,
    container_id: str,
    destination: str,
    platform: str,
    changes: list[str],
    message: str | None,
) -> None:
    import_args = ["image", "import", "--platform", platform]
    if message:
        import_args += ["--message", message]
    for change in changes:
        import_args += ["--change", change]
    import_args += ["-", destination]

    eprint("[2/5] 正在流式合并文件系统（大镜像可能需要较长时间）……")
    exporter = subprocess.Popen(
        docker.prefix + ["container", "export", container_id],
        stdout=subprocess.PIPE,
    )
    assert exporter.stdout is not None
    importer = subprocess.Popen(
        docker.prefix + import_args,
        stdin=exporter.stdout,
    )
    exporter.stdout.close()

    try:
        import_code = importer.wait()
        export_code = exporter.wait()
    except BaseException:
        importer.terminate()
        exporter.terminate()
        importer.wait()
        exporter.wait()
        raise

    if import_code != 0 or export_code != 0:
        raise RuntimeError(
            "文件系统合并失败："
            f"docker export={export_code}, docker import={import_code}"
        )


def apply_image_metadata(
    docker: Docker,
    source: str,
    destination: str,
    platform: str,
    config: dict[str, Any],
    author: str | None,
) -> None:
    with tempfile.TemporaryDirectory(prefix="docker-flatten-") as temp_dir:
        dockerfile = Path(temp_dir) / "Dockerfile"
        lines = [f"FROM {source}"]
        if author:
            lines.append("MAINTAINER " + dockerfile_word(author))
        # WORKDIR is already applied by docker import. Repeating it here can
        # create the directory and therefore an unwanted second rootfs layer.
        lines.extend(
            metadata_changes(
                config,
                include_onbuild=True,
                include_workdir=False,
            )
        )
        dockerfile.write_text("\n".join(lines) + "\n", encoding="utf-8")

        environment = os.environ.copy()
        # The build consists only of metadata instructions. No command is run
        # and no network access is needed.
        docker.run(
            [
                "build",
                "--pull=false",
                "--network=none",
                "--platform",
                platform,
                "--tag",
                destination,
                temp_dir,
            ],
            env=environment,
        )


def semantically_equal(left: Any, right: Any) -> bool:
    if left == right:
        return True
    # Docker versions sometimes normalize an unset list/map as an empty one.
    if left is None and right in ([], {}):
        return True
    if right is None and left in ([], {}):
        return True
    return False


def unique_environment(environment: Any) -> dict[str, str] | None:
    """Return ENV as a mapping, or None when duplicate names need strict order."""
    if environment is None:
        return {}
    if not isinstance(environment, list):
        return None

    result: dict[str, str] = {}
    for item in environment:
        if not isinstance(item, str):
            return None
        name, separator, value = item.partition("=")
        if not separator:
            value = ""
        if name in result:
            return None
        result[name] = value
    return result


def config_value_equal(key: str, left: Any, right: Any) -> bool:
    if semantically_equal(left, right):
        return True
    if key == "Env":
        left_environment = unique_environment(left)
        right_environment = unique_environment(right)
        return (
            left_environment is not None
            and right_environment is not None
            and left_environment == right_environment
        )
    return False


def config_differences(
    source: dict[str, Any], destination: dict[str, Any]
) -> tuple[list[str], list[str]]:
    managed: list[str] = []
    unmanaged: list[str] = []
    for key in sorted(set(source) | set(destination)):
        if config_value_equal(key, source.get(key), destination.get(key)):
            continue
        rendered = (
            f"{key}: 原值={dockerfile_json(source.get(key))}, "
            f"新值={dockerfile_json(destination.get(key))}"
        )
        if key in MANAGED_CONFIG_KEYS:
            managed.append(rendered)
        else:
            unmanaged.append(rendered)
    return managed, unmanaged


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="把 Docker 镜像压平为一个文件系统层，并自动迁移、校验镜像配置。"
    )
    parser.add_argument("source", help="原镜像，例如 repo/app:v1")
    parser.add_argument("target", help="新镜像，例如 repo/app:v1-flat")
    parser.add_argument(
        "--sudo",
        action="store_true",
        help="每条 Docker 命令前加 sudo（也可以直接 sudo 运行本脚本）",
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="允许目标标签已存在；只移动标签，不主动删除旧目标镜像数据",
    )
    parser.add_argument(
        "--keep-temp",
        action="store_true",
        help="失败时保留脚本创建的临时容器/镜像，便于排查",
    )
    parser.add_argument(
        "--docker",
        default="docker",
        metavar="PATH",
        help="Docker CLI 路径，默认 docker",
    )
    parser.add_argument(
        "--version",
        action="version",
        version=f"%(prog)s {SCRIPT_VERSION}",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.source == args.target:
        raise RuntimeError("原镜像和新镜像不能使用相同的引用")
    if "@" in args.target:
        raise RuntimeError("目标必须是可打标签的名称，不能使用 digest 引用")

    docker = Docker(args.sudo, args.docker)
    token = uuid.uuid4().hex[:12]
    rootfs_image = f"codex-flatten-rootfs:{token}"
    configured_image = f"codex-flatten-config:{token}"
    container_id: str | None = None
    temporary_images: list[str] = []
    success = False

    try:
        eprint("[1/5] 正在检查原镜像和目标名称……")
        source_info = docker.inspect(args.source)
        assert source_info is not None
        target_info = docker.inspect(args.target, required=False)
        if target_info is not None and not args.force:
            raise RuntimeError(
                f"目标镜像 {args.target} 已存在；请更换名称，或确认后添加 --force"
            )

        config = source_info.get("Config") or {}
        platform = platform_of(source_info)

        # Supplying a harmless command lets images without a default CMD be
        # created too. The container is never started, so the command need not
        # exist inside the image and cannot change the exported filesystem.
        created = docker.run(
            ["container", "create", args.source, "__codex_flatten_noop__"],
            capture=True,
        ).stdout.strip()
        if not created:
            raise RuntimeError("docker create 没有返回容器 ID")
        container_id = created

        message = source_info.get("Comment") or None
        # docker import treats --change as configuration-only metadata. Apply
        # WORKDIR here so a later Dockerfile WORKDIR instruction cannot create
        # a missing directory as a second filesystem layer.
        import_changes: list[str] = []
        if config.get("WorkingDir"):
            import_changes.append(
                "WORKDIR " + dockerfile_word(config["WorkingDir"])
            )
        temporary_images.append(rootfs_image)
        stream_export_import(
            docker,
            container_id,
            rootfs_image,
            platform,
            import_changes,
            message,
        )

        eprint("[3/5] 正在补充其余镜像元数据……")
        temporary_images.append(configured_image)
        apply_image_metadata(
            docker,
            rootfs_image,
            configured_image,
            platform,
            config,
            source_info.get("Author") or None,
        )
        candidate = configured_image

        eprint("[4/5] 正在逐项校验配置和层数……")
        new_info = docker.inspect(candidate)
        assert new_info is not None
        new_config = new_info.get("Config") or {}
        managed_diff, unmanaged_diff = config_differences(config, new_config)

        layers = ((new_info.get("RootFS") or {}).get("Layers") or [])
        if len(layers) != 1:
            raise RuntimeError(
                f"校验失败：新镜像有 {len(layers)} 个文件系统层，预期为 1 个"
            )
        if managed_diff:
            details = "\n  - ".join(managed_diff)
            raise RuntimeError("关键镜像配置校验失败：\n  - " + details)

        docker.run(["image", "tag", candidate, args.target])

        eprint("[5/5] 完成。")
        old_size = int(source_info.get("Size") or 0)
        new_size = int(new_info.get("Size") or 0)
        old_layers = len(((source_info.get("RootFS") or {}).get("Layers") or []))
        print(f"新镜像：{args.target}")
        print(f"平台：{platform}")
        print(f"文件系统层：{old_layers} -> {len(layers)}")
        print(f"镜像大小：{human_size(old_size)} -> {human_size(new_size)}")
        print("关键运行配置：迁移并校验通过")

        if unmanaged_diff:
            print("\n以下非 Dockerfile 运行配置字段无法保证等值（通常是历史兼容字段）：")
            for difference in unmanaged_diff:
                print("  - " + difference)

        print(
            "\n说明：镜像 ID、digest、父层关系和构建历史必然会改变；"
            "它们与‘压成一层’本身互斥。"
        )
        success = True
        return 0
    finally:
        if not args.keep_temp or success:
            if container_id:
                docker.run(
                    ["container", "rm", "--force", "--volumes", container_id],
                    capture=True,
                    check=False,
                    quiet_error=True,
                )
            for image in reversed(temporary_images):
                docker.run(
                    ["image", "rm", image],
                    capture=True,
                    check=False,
                    quiet_error=True,
                )
        elif not success:
            eprint("已按 --keep-temp 保留临时对象：")
            if container_id:
                eprint(f"  容器：{container_id}")
            for image in temporary_images:
                eprint(f"  镜像：{image}")


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        eprint("\n操作已中断。")
        raise SystemExit(130)
    except RuntimeError as exc:
        eprint(f"错误：{exc}")
        raise SystemExit(1)
    except FileNotFoundError as exc:
        eprint(f"错误：找不到命令：{exc.filename}")
        raise SystemExit(127)
