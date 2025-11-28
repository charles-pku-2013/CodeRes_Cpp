class NpuMgr:
    """使用 ProcessPoolExecutor 实现多进程并行推理。"""
    def __init__(self, npu_type_class, *args):
        self.npu_type_class = npu_type_class
        # 调用之前实现的函数来获取 NPU 数量
        n_npus = self.get_num_npus()

        self.npus = []

        # 构造所有 npu 对象
        for i in range(n_npus):
            # 将可变参数 args 解包，并将 device_id 作为命名参数传递
            npu_instance = self.npu_type_class(*args, device_id=i)
            self.npus.append(npu_instance)

        dev_list = [npu.device_id() for npu in self.npus]
        logger.info(f"Successfully initialized npu devices: {', '.join(dev_list)}")

    def get_num_npus(self) -> int:
        n_npus = 0
        device_base_path = self.npu_type_class.device_path()

        while True:
            # path = os.path.join(device_base_path, f"{n_npus}")
            path = f"{device_base_path}-{n_npus}"
            # print(f"path = {path}")

            if not os.path.exists(path):
                break

            n_npus += 1

        return n_npus

    def _worker_inference(self, npu_instance, imgs_part, *args):
        """一个独立的函数，用于在子进程中执行 NPU 的推理方法。"""
        # 这个函数需要是顶层的（非嵌套的），以便 multiprocessing 能够对其进行序列化和分派
        try:
            return npu_instance.inference(imgs_part, *args)
        except Exception as ex:
            logger.error(f"Error happened during inference of NPU {npu_instance.device_id()}: {ex}")
            raise # 重新抛出异常，让主进程感知

    def inference(self, imgs, *args):
        if not imgs:
            return []

        n_imgs = len(imgs)
        n_parts = len(self.npus)
        base_size = n_imgs // n_parts
        remainder = n_imgs % n_parts

        logger.info(f"NpuMgr received a {n_imgs} image inference task")

        results = []
        # 使用 ProcessPoolExecutor 管理子进程池
        with concurrent.futures.ProcessPoolExecutor(max_workers=n_parts) as executor:
            futures = []
            range_beg = 0

            # 将输入数组 imgs 拆分成 n_parts 等份，每一份交给一个子进程去处理
            for i in range(n_parts):
                part_size = base_size + (1 if i < remainder else 0)
                range_end = range_beg + part_size
                imgs_part = imgs[range_beg:range_end]
                npu_instance = self.npus[i]

                logger.info(f"NpuMgr assigning items {range_beg} ... {range_end} to NPU {npu_instance.device_id()}")
                # 提交任务到进程池，并将 future 对象存储起来
                future = executor.submit(self._worker_inference, npu_instance, imgs_part, *args)
                futures.append(future)

                range_beg = range_end

            # time.sleep(10)
            # 合并子进程的返回结果
            for i, future in enumerate(futures):
                try:
                    # 使用 .result() 获取返回值，这会阻塞直到任务完成
                    result = future.result()
                    results.extend(result) # 合并结果
                except Exception:
                    # 异常已经在 _worker_inference 中记录并重新抛出
                    # 这里可以捕获并决定如何处理，例如再次抛出或返回部分结果
                    # 遵循原 C++ 代码的逻辑，这里再次抛出
                    raise

        return results

用上述代码实现了NpuMgr，但现在有个问题，因为 npu_type_class 不能被 pickle，所以无法将在主进程里创建好的 npu_type_class 对象传递给子进程。请问如何解决？

创建 npu_instance 实例开销巨大，我希望子进程创建好npu_instance实例后，像服务器一样保持运行状态，等待用户数据待处理的数据并实时返回处理结果。该如何实现？
