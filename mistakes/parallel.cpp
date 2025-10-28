    template <typename Container, typename... Args>
    ResultType inference(const Container& imgs, Args&&... args) {
        // 初始化python环境
        py::array_t<float> fake{};
        (void*)(fake.mutable_data());

        // pybind11::gil_scoped_release release;
        // DLOG(INFO) << "Has GIL: " << PyGILState_Check();

        if (npus_.empty()) {
            throw std::runtime_error("No hardware available!");
        }

        if (imgs.empty()) {
            throw std::runtime_error("Empty input!");
        }

        LOG(INFO) << fmt::format("NpuMgr received a {} images processing task", imgs.size());

        std::size_t n_imgs = imgs.size();
        std::size_t num_parts = npus_.size();
        std::size_t base_size = n_imgs / num_parts;
        std::size_t remainder = n_imgs % num_parts;

        std::vector<ResultType> results;
        results.resize(num_parts);

        auto current_it = imgs.begin();

        // 将输入数据数组分成num_parts等份并行处理
#pragma omp parallel for
        for (std::size_t i = 0; i < num_parts; ++i) {
            std::size_t part_size = base_size + (i < remainder ? 1 : 0);
            auto        end_it = current_it + part_size;
            LOG(INFO) << fmt::format("NpuMgr assigning {} ... {} to NPU {}",
                                     std::distance(imgs.begin(), current_it),
                                     std::distance(imgs.begin(), end_it), npus_[i]->device_id());
            results[i] = npus_[i]->inference_impl(current_it, end_it,
                                                  std::forward<Args>(args)...);  // RVO takes effect
            current_it += part_size;   // WRONG!!!
        }

        // 合并到最终结果
        ResultType res;
        res.reserve(n_imgs);

        for (auto& result : results) {
            std::copy(std::make_move_iterator(result.begin()),
                      std::make_move_iterator(result.end()), std::back_inserter(res));
        }

        assert(res.size() == n_imgs);

        // DLOG(INFO) << fmt::format("Result addr check 2: {:#x} {:#x}", (uint64_t)(res[0].data()),
                                  // (uint64_t)(res[0][0].mutable_data()));

        return res;
    }
