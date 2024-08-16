#include "bthread/bthread.h"

// NOTE 不可以bthread pthread/std::thread 混用，否则就是普通线程 只能用 n_cpu+1 个工作线程
// bthread_getconcurrency bthread_setconcurrency 查看更改设置

namespace {
class BThreadTest {
 public:
    struct ThreadArgs {
        typedef void (BThreadTest::*Func)(int64_t);   // mem func pointer type
        Func pfn = nullptr;  // pointer to mem func
        BThreadTest* inst = nullptr;  // this pointer
        int64_t idx = 0;     // mem func args
    };

    void thread_routine(int64_t idx) {
        LOG(INFO) << "Thread " << bthread_self() << " running...";
        bthread_usleep(10 * 1000 * 1000);  // 10s  NOTE!!! cannot use ::sleep or std::bthread_usleep ...
        // LOG(INFO) << "Thread " << idx << " end.";
    }

    static
    void* thread_fun(void* _args) {
        ThreadArgs* args = reinterpret_cast<ThreadArgs*>(_args);
        bthread_t this_tid = bthread_self();
        if (!args || !args->inst) {
            return nullptr;
        }
        args->inst->on_thread_start(this_tid, args);
        if (args->pfn) {
            (args->inst->*args->pfn)(args->idx);   // call func in this way
        }
        args->inst->on_thread_finish(this_tid);
        return nullptr;
    }

    void run() {
        bthread_t join_tid = 0;
        bthread_start_background(&join_tid, nullptr, &BThreadTest::join_threads, this);
        int64_t idx = 0;
        while (true) {
            // E0814 15:05:11.904269 12316 task_group.cpp:708] _remote_rq is full, capacity=2048
            // 最多同时运行30000+个bthread
            bthread_t tid = 0;
            ThreadArgs* arg = new ThreadArgs{&BThreadTest::thread_routine, this, idx};
            int err = bthread_start_background(&tid, nullptr, &BThreadTest::thread_fun, arg);
            if (err) {
                LOG(ERROR) << "Failed to start bthread " << idx;
                continue;
            }
            ++idx;
            if (idx > 10) { break; }
        }
        ::sleep(3600);
    }

    void on_thread_start(bthread_t tid, ThreadArgs *arg) {
        std::lock_guard<bthread::Mutex> lck(lock_running_threads_);
        running_threads_[tid].reset(arg);
    }

    void on_thread_finish(bthread_t tid) {
        {
            std::lock_guard<bthread::Mutex> lck(lock_running_threads_);
            auto cnt = running_threads_.erase(tid);
            if (cnt == 0) { return; }
        }
        {
            std::lock_guard<bthread::Mutex> lck(lock_finished_threads_);
            finished_threads_.emplace_back(tid);
            cond_finished_threads_.notify_all();
        }
    }

    void _join_threads() {
        while (true) {
            std::unique_lock<bthread::Mutex> lck(lock_finished_threads_);
            while (finished_threads_.empty()) {
                cond_finished_threads_.wait(lck);
            }
            bthread_t tid = finished_threads_.front();
            finished_threads_.pop_front();
            lck.unlock();  // NOTE!!! must unlock, for join is long time op
            bthread_join(tid, nullptr);
            LOG(INFO) << "Thread " << tid << " done.";
        }
    }

    static
    void* join_threads(void *arg) {
        BThreadTest* inst = reinterpret_cast<BThreadTest*>(arg);
        inst->_join_threads();
        return nullptr;
    }

 private:
    std::unordered_map<bthread_t, std::unique_ptr<ThreadArgs>> running_threads_;
    bthread::Mutex lock_running_threads_;

    std::deque<bthread_t> finished_threads_;
    bthread::Mutex lock_finished_threads_;
    bthread::ConditionVariable cond_finished_threads_;
};
}  // namespace
