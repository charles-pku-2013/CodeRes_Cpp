import p220
import concurrent.futures
import numpy as np
import multiprocessing as mp
import os
import time

# --- Configuration (保持一致) ---
SNAP_PATH = "/path/to/your/model.snap"
MEAN_VAL = 0.0
STD_VAL = 1.0

def create_dummy_images(num_images, img_shape=(224, 224, 3)):
    """Helper to generate dummy data."""
    return [np.random.randint(0, 255, size=img_shape, dtype=np.uint8) for _ in range(num_images)]

# 核心 Worker 函数的修改
def inference_worker_server(task_queue: mp.Queue, result_queue: mp.Queue, device_id: int):
    """
    一个常驻子进程，初始化 P220 对象一次，然后循环处理队列中的任务。
    """
    pid = os.getpid()
    print(f"[PID {pid}] Worker 进程启动，正在初始化 P220 engine (Device ID: {device_id})...")
    
    try:
        # !!! 关键步骤：在子进程启动后，只构造一次 P220 对象 !!!
        engine = p220.P220(
            snap_path=SNAP_PATH,
            upload_format="NHWC",
            download_format="NCHW",
            device_id=device_id
        )
        print(f"[PID {pid}] Engine 初始化完成，使用设备: {engine.device_id()}. 准备接收任务。")

        while True:
            # 阻塞等待新任务
            task = task_queue.get()
            
            if task is None:
                # 收到 None 信号，表示任务结束，退出循环
                print(f"[PID {pid}] 收到终止信号，Worker 退出。")
                break
            
            # task 应该包含 (unique_task_id, image_batch)
            task_id, image_batch = task
            
            # 执行推理
            results = engine.inference_np(
                image_batch,
                MEAN=MEAN_VAL,
                STD=STD_VAL,
                skip_resize=True
            )
            
            # 将结果和任务ID一起放回结果队列
            result_queue.put((task_id, results))

    except Exception as e:
        print(f"[PID {pid}] 发生致命错误: {e}")
        # 如果出错，也放入一个错误标识到结果队列，让主进程知道
        result_queue.put((None, f"ERROR in PID {pid}: {e}"))
        
    print(f"[PID {pid}] Worker 进程完全结束。")


# --- Main Execution ---
if __name__ == '__main__':
    # 使用 'spawn' 启动方法通常更健壮，尤其是在混合 C++ pybind11 和多进程时
    # mp.set_start_method('spawn', force=True) 

    NUM_WORKERS = 2
    
    # 创建任务和结果队列
    tasks_queue = mp.Queue()
    results_queue = mp.Queue()
    
    # 启动子进程（工人）
    processes = []
    for i in range(NUM_WORKERS):
        p = mp.Process(
            target=inference_worker_server, 
            args=(tasks_queue, results_queue, -1) # 使用-1让C++代码自动分配设备
        )
        p.start()
        processes.append(p)
    
    time.sleep(1) # 给子进程一点时间初始化

    # --- 主进程：生成并分发任务 ---
    all_images = create_dummy_images(60) # 60 张图片
    batch_size = 10
    batches = [all_images[i:i + batch_size] for i in range(0, len(all_images), batch_size)]
    
    print(f"\n主进程：总共 {len(batches)} 个批次等待处理。")

    # 将所有任务放入输入队列，并给每个任务一个唯一的ID
    for i, batch in enumerate(batches):
        tasks_queue.put((i, batch))
        print(f"主进程：已提交任务 ID {i}")

    # --- 主进程：收集结果 ---
    collected_results = {}
    
    # 循环直到收集到所有预期结果
    while len(collected_results) < len(batches):
        # 阻塞等待结果返回
        task_id, results = results_queue.get()
        if task_id is not None:
            collected_results[task_id] = results
            print(f"主进程：收到任务 ID {task_id} 的结果。")
        else:
            print(f"主进程：收到错误信息，请检查子进程日志。")

    print("\n主进程：所有任务均已完成。")
    
    # --- 主进程：发送终止信号给所有子进程 ---
    for _ in range(NUM_WORKERS):
        tasks_queue.put(None)
        
    # 等待所有子进程安全退出
    for p in processes:
        p.join()
        
    print("所有子进程已关闭。主程序退出。")

