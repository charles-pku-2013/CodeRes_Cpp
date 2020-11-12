// from dict
while (!_LoadServables()) {
    std::unique_lock<std::mutex> lk(updating_mutex_);
    if (!updating_cond_.wait_for(lk, std::chrono::hours(3),
                [this]{ return n_to_load_ <= 0; })) {
        // ❌❌❌ 要区分wait失败和条件满足
        // wait有两种退出条件 1.超时(失败) 2.条件满足，返回true
        if (!updating_) {
            return;
        }
        LOG(ERROR) << "ServableManager updating fail for waiting too long!";
        updating_tp_->Cancel();
        updating_tp_.reset();
        return;
    }
}
