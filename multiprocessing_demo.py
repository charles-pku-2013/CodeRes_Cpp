# Source - https://stackoverflow.com/a
# Posted by vartec, modified by community. See post 'Timeline' for change history
# Retrieved 2025-11-27, License - CC BY-SA 4.0

import multiprocessing, time, os


def worker(procnum, return_dict):
    """worker function"""
    print(f"Process {procnum} {os.getpid()} running...")
    return_dict[procnum] = procnum
    time.sleep(20)
    print(f"Process {procnum} {os.getpid()} done")


if __name__ == "__main__":
    print(f"main pid = {os.getpid()}")

    manager = multiprocessing.Manager()
    return_dict = manager.dict()
    jobs = []
    for i in range(5):
        p = multiprocessing.Process(target=worker, args=(i, return_dict))
        jobs.append(p)
        p.start()

    for proc in jobs:
        proc.join()
    print(return_dict.values())
