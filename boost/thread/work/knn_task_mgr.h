#ifndef _KNN_TASK_MGR_H_
#define _KNN_TASK_MGR_H_

#include "tensorflow_serving/servables/knn/knn_task.h"

namespace knn {

class KnnTaskBatch final : public Notifiable {
public:
    using pointer = std::shared_ptr<KnnTaskBatch>;
    enum Status { READY, COMPUTING };
public:
    KnnTaskBatch(IndexType *idx, int batch_size, int dimension, int topk, int timeout)
            : status_(READY),
              pos_(0),
              running_(true),
              index_(idx),
              batch_size_(batch_size),
              dimension_(dimension),
              topk_(topk),
              TIMEOUT(timeout)
    {
        data_.resize(batch_size_ * dimension_);
        labels_.resize(batch_size_ * topk_);
        scores_.resize(batch_size_ * topk_);
        tasks_.resize(batch_size_);
        work_thread_.reset(new std::thread(&KnnTaskBatch::_work_routine, this));
    }

    ~KnnTaskBatch() {
        _shutdown();
        work_thread_->join();
    }

    bool add_task(const KnnTask::pointer &task, int &idx) {
        if (status_ != READY || pos_ >= batch_size_) { return false; }
        // put data
        boost::shared_lock<KnnTaskBatch> lck(*this, boost::defer_lock);
        if (!lck.try_lock()) { return false; }
        int pos = pos_++;
        if (pos >= batch_size_) { return false; }
        tasks_[pos] = task;
        lck.unlock();

        if (pos == 0) // notify task available
        { cond_has_task_.notify_all(); }
        if (pos == batch_size_-1)
        { _notify_work(); }

        idx = pos;
        // LOG(INFO) << "KnnTaskBatch add task success " << boost::format("%d %lx") % pos % (void*)(task.get()); // debug
        return true;
    }

    IndexType* index() { return index_; }

    std::string DebugString() const;

private:
    void _notify_work() {
        status_ = COMPUTING;
        cond_.notify_all();
    }

    void _work_routine();

    void _do_work();

    void _shutdown() {
        running_ = false;
        cond_has_task_.notify_all();
        cond_.notify_all();
    }

private:
    std::atomic_int status_;
    std::atomic_int pos_;
    std::atomic_bool running_;
    IndexType *index_;
    const int batch_size_;
    const int dimension_;
    const int topk_;
    const int TIMEOUT;
    std::vector<float> data_;
    std::vector<IndexIdx> labels_;
    std::vector<float> scores_;
    std::vector<KnnTask::pointer> tasks_;
    std::unique_ptr<std::thread> work_thread_;
    std::condition_variable_any cond_has_task_;
};


class KnnTaskMgr final {
public:
    KnnTaskMgr(IndexType *idx, int sz, int bsz, int d, int topk, int timeout)
        : index_(idx), size_(sz), batch_size_(bsz), dimension_(d), topk_(topk), timeout_(timeout), pos_(0)
    {
        for (int i = 0; i < size_; ++i)
        { workers_.push_back(std::make_shared<KnnTaskBatch>(index_, batch_size_, dimension_, topk_, timeout_)); }
    }

    // ~KnnTaskMgr()
    // { LOG(INFO) << "KnnTaskMgr " << DebugString() << " destruct"; }

    void add_task(const KnnTask::pointer &task) {
        bool success = false;
        int fail_cnt = 0;
        int pos = pos_;
        do {
            int idx = 0;
            auto& pTaskBatch = workers_[pos];
            success = pTaskBatch->add_task(task, idx);
            if (success) {
                // LOG(INFO) << "KnnTaskMgr add task success " << boost::format("%d %d %lx %lx") % pos % idx % (void*)(pTaskBatch.get()) % (void*)(task.get()); // debug
                if (pos != pos_) { pos_ = pos; }
                return;
            }
            // LOG(INFO) << "add task fail " << boost::format("%lx %lx %d") % (void*)(pTaskBatch.get()) % (void*)(task.get()) % pos;
            pos = (pos + 1) % size_;
            if (++fail_cnt == size_) {
                LOG(INFO) << "Insufficient worker thread! n_worker_threads=" << size_;
                fail_cnt = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(5)); // sleep 5ms
            }
        } while (!success);
    }

    int topk() const { return topk_; }

    IndexType* index() { return index_; }

    std::string DebugString() const;

private:
    IndexType *index_ = nullptr;
    int size_;
    int batch_size_;
    int dimension_;
    int topk_;
    const int timeout_; // in ms
    std::atomic_int pos_;
    std::vector<KnnTaskBatch::pointer> workers_;
};

} // namespace knn

#endif // _KNN_TASK_MGR_H_

