#ifndef _KNN_TASK_H_
#define _KNN_TASK_H_

#include <atomic>
#include <algorithm>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include "tensorflow/core/platform/logging.h"
#include "faiss/Index.h"

namespace knn {

using IndexType = faiss::Index;
using IndexIdx = faiss::Index::idx_t;

class Notifiable : public boost::shared_lockable_adapter<boost::shared_mutex> {
public:
    virtual ~Notifiable() = default;
    virtual void wait() {}
    virtual void notify() {}
protected:
    std::condition_variable_any cond_;
};

// 由请求线程提交
class KnnTask final : public Notifiable {
public:
    using pointer = std::shared_ptr<KnnTask>;
public:
    KnnTask() : done_(false) {}

    virtual ~KnnTask() = default;

    void set_data(const float *_data) { data_ = _data; }
    const float* data() const { return data_; }

    void set_labels(const IndexIdx* data, int len)
    { labels_.assign(data, data + len); }
    std::vector<IndexIdx>& labels() { return labels_; }

    void set_scores(const float* data, int len)
    { scores_.assign(data, data + len); }
    std::vector<float>& scores() { return scores_; }

    void wait() override {
        boost::shared_lock<KnnTask> lck(*this);
        cond_.wait(lck, [&, this]()->bool{ return done_; });
    }

    void notify() override {
        done_ = true;
        cond_.notify_all();
    }

private:
    std::atomic_bool done_;
    const float* data_ = nullptr;
    std::vector<IndexIdx> labels_;
    std::vector<float> scores_;
};

} // namespace knn

#endif // _KNN_TASK_H_

