// before
bool PredictRequestUtil::warmup(TFSPredictor *predictor, int specfic_version) {
    if (predictor->IsTfLite()) {
        LOG(INFO) << predictor->DebugString() << " warm_up raw input.";
        return bundleRaw(predictor, specfic_version);
    }
    if (!predictor->IsLocal()) {
        return true;
    }
    if (predictor->UsePointer()) {
        LOG(INFO) << predictor->DebugString() << " warm_up feature_bundle.";
        for (int i = 0; i < 10; i++) {
            bool flag = bundle(predictor, specfic_version);
            if (!flag) {   // 如果有一次失败，则返回false
                return false;
            }
        }
        return true;
    } else {
        LOG(INFO) << predictor->DebugString() << " please use tfs warm_up.";
        return true;
    }
}

// after
bool PredictRequestUtil::warmup(TFSPredictor *predictor, int specfic_version) {
    if (tfs_predictor_warmup_jobs_.count(predictor) > 0) {
        return true;
    }
    LOG(INFO) << "Warming up " << predictor->DebugString();

    auto job = std::make_shared<TFSPredictorWarmupJob>();
    job->worker = std::thread(
        [&status = job->status, predictor, specfic_version](){
            if (predictor->IsTfLite()) {
                LOG(INFO) << predictor->DebugString() << " warm_up raw input.";
                status = bundleRaw(predictor, specfic_version);
                return;  // NOTE!!! You cannot only changle return to set value
            }
            if (!predictor->IsLocal()) {
                status = true;
                return;  // REQUIRED
            }
            if (predictor->UsePointer()) {
                LOG(INFO) << predictor->DebugString() << " warm_up feature_bundle.";
                for (int i = 0; i < 10; i++) {
                    bool flag = bundle(predictor, specfic_version);
                    if (!flag) {   // 如果有一次失败，则返回false
                        status = false;
                        return;
                    }
                }
                status = true;
                return;
            } else {
                LOG(INFO) << predictor->DebugString() << " please use tfs warm_up.";
                status = true;
                return;
            }
        }
    );

    tfs_predictor_warmup_jobs_[predictor] = job;
    return true;
}
