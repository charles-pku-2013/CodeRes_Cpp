

struct RwLock {
    Mutex mtx;
    CondVar cond;

    atomic_bool reading = false, writting = false;
    atomic_size_t rdCount = 0;

    void rdLock()
    {
        unique_lock lck(mtx);
        while (writting)
            cond.wait(lck);
        reading = true;
        ++rdCount;
    }

    void rdUnlock()
    {
        unique_lock lck(mtx);
        if (--rdCount == 0) {
            reading = fasle;
            lck.unlock();
            cond.notify_all();
        } // if
    } 

    void wrLock()
    {
        unique_lock lck(mtx);
        while (writting || reading)
            cond.wait(lck);
        writting = true;
    }

    void wrUnLock()
    {
        unique_lock lck(mtx);
        writting = false;
        lck.unlock();
        cond.notify_all();
    }
};


