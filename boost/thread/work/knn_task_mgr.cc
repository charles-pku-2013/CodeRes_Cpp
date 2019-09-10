#include "tensorflow_serving/servables/knn/knn_task_mgr.h"
#include <sstream>
#ifndef _WIN32
#include <sys/prctl.h>
#endif

namespace knn {

void KnnTaskBatch::_work_routine() {
    #ifndef _WIN32
    prctl(PR_SET_NAME, "knn_task_batch_routine");
    #endif
    // LOG(INFO) << "Knn work routine working..."; // debug
    while (running_) {
        // wait until at least 1 task available
        do {
            boost::shared_lock<KnnTaskBatch> lck(*this);
            cond_has_task_.wait(lck, [&, this]{ return (pos_ > 0 || !running_); });
        } while (0);

        if (!running_ && pos_ <= 0) { return; }

        // wait to fill task buffer
        do {
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(TIMEOUT);
            boost::shared_lock<KnnTaskBatch> slck(*this);
            cond_.wait_until(slck, deadline, [&, this]{ return (status_ == COMPUTING || !running_); });
        } while (0);

        // do computing
        do {
            boost::unique_lock<KnnTaskBatch> ulck(*this);
            status_ = COMPUTING;
            _do_work();
            pos_ = 0;
            status_ = READY;
        } while (0);
    }
}

void KnnTaskBatch::_do_work() {
    try {
        // first collect data
        int i = 0;
        for (; i < batch_size_ && tasks_[i]; ++i) {
            const float* data = tasks_[i]->data();
            std::copy(data, data + dimension_, data_.begin() + i * dimension_);
        }
        // then computing
        if (i == 0) { return; }
        // debug
        // if (i < batch_size_) {
            // LOG(INFO) << "Insufficient computing " << boost::format("%d %lx") % i % (void*)this;
        // }
        index_->search(
            i,
            data_.data(),
            topk_,
            scores_.data(),
            labels_.data()
        );
        // finally set resp & notify
        for (i = 0; i < batch_size_ && tasks_[i]; ++i) {
            tasks_[i]->set_labels(labels_.data() + i * topk_, topk_);
            tasks_[i]->set_scores(scores_.data() + i * topk_, topk_);
            tasks_[i]->notify();
            tasks_[i].reset();
        }
    } catch (const std::exception &ex) {
        LOG(ERROR) << "KnnTaskBatch::_do_work error: " << ex.what();
    } catch (...) {
        LOG(ERROR) << "KnnTaskBatch::_do_work unknown error!";
    }
}

std::string KnnTaskMgr::DebugString() const {
    std::ostringstream oss;
    oss << "KnnTaskMgr: idx_addr=" << boost::format("%lx") % (void*)(index_)
        << " n_tasks=" << size_ << " batch_size=" << batch_size_
        << " dimension=" << dimension_ << " default_topk=" << topk_
        << " batch_timeout=" << timeout_ << std::flush;
    return oss.str();
}

std::string KnnTaskBatch::DebugString() const {
    std::ostringstream oss;
    oss << "KnnTaskBatch: idx_addr=" << boost::format("%lx") % (void*)(index_)
        << " batch_size=" << batch_size_
        << " dimension=" << dimension_ << " default_topk=" << topk_
        << " batch_timeout=" << TIMEOUT << std::flush;
    return oss.str();
}

} // namespace knn
