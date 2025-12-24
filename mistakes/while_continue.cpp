int id = 0;
while (true) {
    std::string path = fmt::sprintf("%s-%d", NpuType::device_path(), id);
    DLOG(INFO) << "Detecting " << path;

    if (!fs::exists(path)) {
        return;
    }
    if (is_busy(id)) {
        continue;  // NOTE!!! infinite loop! ++id 不会被执行
    }

    auto processor = std::make_shared<NpuProcessor>();
    processor->id_ = id;
    processor->dev_id_ = std::move(path);
    npu_processors_.emplace_back(std::move(processor));

    ++id;
}  // while

// Correct
for (int id = 0; true; ++id)
