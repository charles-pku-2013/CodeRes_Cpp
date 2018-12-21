struct KnnBuffer {
    enum Status {
        READY,
        COMPUTING,
        COMPUTE_DONE,
    };

    KnnBuffer(int batch, int d, int topk)
            : batch_size_(batch),
              dimension_(d),
              topk_(topk),
              pos_(0),
              num_data_(0),
              num_done_(0),
              status_(READY)
    {
        data_.resize(batch_size_ * dimension_);
        labels_.resize(batch_size_ * topk_);
        scores_.resize(batch_size_ * topk_);
    }

    int set_data(int pos, const float *data) {
        std::copy(data, data + dimension_, data_.begin() + pos * dimension_);
        return ++num_data_;
    }

    void get_labels(int pos, faiss::Index::idx_t *labels) {
        auto start = labels_.begin() + pos * topk_;
        auto end = start + topk_;
        std::copy(start, end, labels);
    }

    void get_scores(int pos, float *scores) {
        auto start = scores_.begin() + pos * topk_;
        auto end = start + topk_;
        std::copy(start, end, scores);
    }

    void set_status(int stat)
    { status_ = stat; }
    int status() const { return status_; }

    // int get_pos() { return pos_++; }

    int wait_get_pos(const int timeout) {
        int cnt = 0;
        while (true) {
            if (status_ == READY) {
                int pos = pos_++;
                if (pos < batch_size_)
                    return pos;
            }
            SLEEP_MICROSECONDS(5);
            cnt += 5;
            if (cnt >= timeout) {
                return -1;
            }
        } // while
    }

    void done() {
        if (++num_done_ >= batch_size_) {
            reset();
        }
    }

    void reset() {
        pos_ = 0;
        num_data_ = 0;
        num_done_ = 0;
        status_ = READY;
    }

    int batch_size() const { return batch_size_; }
    int topk() const { return topk_; }
    int dimension() const { return dimension_; }
    float* data() { return data_.data(); }
    float* scores() { return scores_.data(); }
    faiss::Index::idx_t* labels() { return labels_.data(); }

    const int batch_size_ = 0;
    const int dimension_ = 0;
    const int topk_ = 0;

    std::atomic_int pos_;
    std::atomic_int num_data_;
    std::atomic_int num_done_;
    std::atomic_int status_;

    std::vector<float> data_;
    std::vector<faiss::Index::idx_t> labels_;
    std::vector<float> scores_;
};


void KnnPredServiceImpl::_compute_with_buffer(
                    ServableHandle<faiss::Index> &knn_bundle,
                    const float *embedding_data,
                    std::vector<float> &scores,
                    std::vector<faiss::Index::idx_t> &labels) {
    bool done = false;
    int pos = buffer_->wait_get_pos(KNN_TIMEOUT);
    if (pos < 0) {
        // LOG(INFO) << "Wait pos timeout, Do single computing...";
        knn_bundle->search(1, embedding_data, buffer_->topk(), scores.data(), labels.data());
        done = true;
    } else { // buffer must be in READY
        int n = buffer_->set_data(pos, embedding_data);
        // do batch search
        if (n >= buffer_->batch_size()) {
            buffer_->set_status(KnnBuffer::COMPUTING);
            knn_bundle->search(
                buffer_->batch_size(),
                buffer_->data(),
                buffer_->topk(),
                buffer_->scores(),
                buffer_->labels()
            );
            buffer_->set_status(KnnBuffer::COMPUTE_DONE);
        }
        // wait result
        for (int cnt = 0; cnt < KNN_TIMEOUT && buffer_->status() != KnnBuffer::COMPUTE_DONE; cnt += 3) {
            SLEEP_MICROSECONDS(3);
        }
        if (buffer_->status() == KnnBuffer::COMPUTE_DONE) {
            buffer_->get_labels(pos, labels.data());
            buffer_->get_scores(pos, scores.data());
            done = true;
        }
        buffer_->done(); // wait result timeout also set done, because data has been put in
    }

    if (!done) { // wait result timeout
        // LOG(INFO) << "Wait result timeout, Do single computing...";
        knn_bundle->search(1, embedding_data, buffer_->topk(), scores.data(), labels.data());
    }
}
