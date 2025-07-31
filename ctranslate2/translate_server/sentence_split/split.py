# -*- coding: utf-8 -*-
#!/usr/bin/env python

# prepare
# pip3 install fastapi uvicorn regex

# compile bin executable
# pyinstaller -F split.py

# python3 split.py --nobreak_file ./nonbreaking_prefix.en  --host 0.0.0.0 --port 7003
# curl -X POST "http://127.0.0.1:7003/split" -H "Content-Type: application/json" -d '{"lang": "en", "text": "This is a machine translation program running on NPU of Huawei server. Hello. How are you today."}'

from fastapi import FastAPI
from pydantic import BaseModel
from typing import List, Optional
import uvicorn
import argparse
import time

from util.split_sentences import SentenceSplitter

app = FastAPI()

# 定义 Pydantic 模型：请求格式
class SplitRequest(BaseModel):
    lang: str
    text: str

# 定义 Pydantic 模型：响应格式
class SplitResponse(BaseModel):
    #  lang: str
    #  text: str
    sentences: List[str]

# 全局变量：不换行词表路径（从命令行参数中获取）
g_nobreak_file: Optional[str] = None

@app.post("/split", response_model=SplitResponse)
async def split_sentences(req: SplitRequest) -> SplitResponse:
    """
    接收一个 JSON 对象，包含两个字段：
      - lang: "en"、"zh" 等
      - text: 需要切分的一段文本（单条字符串）
    
    返回格式也是一个 JSON：
    {
        "lang": "...",
        "text": "...",
        "sentences": [ "...", "...", ... ]
    }
    """
    # 根据请求中的 lang 动态初始化 SentenceSplitter
    # 也可以根据业务需求考虑将不同 lang 的 splitter 缓存起来，避免每次都重新加载。
    splitter = SentenceSplitter({
        "language": req.lang,
        "nobreak_file": g_nobreak_file
    })

    # 分句
    subs: List[str] = splitter.split_sentences(req.text)

    # 按照新的响应模型返回
    return SplitResponse(
        #  lang=req.lang,
        #  text=req.text,
        sentences=subs
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sentence Split API Server")
    parser.add_argument(
        "--nobreak_file", type=str, required=True, help="不在此处换行的词列表文件路径。"
    )
    parser.add_argument(
        "--host", type=str, required=True, help="Server 监听的 IP 地址。"
    )
    parser.add_argument(
        "--port", type=int, required=True, help="Server 监听的端口号。"
    )
    args = parser.parse_args()

    # 将 nobreak_file 保存在全局变量里，供每次请求时加载
    g_nobreak_file = args.nobreak_file

    # 打印加载耗时（可选）
    start_time = time.time()
    end_time = time.time()
    print(f"========>> 程序启动，准备监听 {args.host}:{args.port}，不换行词表：{g_nobreak_file}")

    # 启动 Uvicorn
    uvicorn.run(app, host=args.host, port=args.port)

