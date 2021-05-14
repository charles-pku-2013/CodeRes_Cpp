class Foo {
    using Executor = std::function<void(void)>;
    static void* _BthreadHelperFunc(void *_arg) {
        Executor *exec = reinterpret_cast<Executor*>(_arg);
        (*exec)();
        return nullptr;
    }
};

int Foo::GetData(
            const std::unordered_set<std::string> &searcher_types,
            const Request &request,
            ResponseSet *responses,
            ResponseStatusSet *response_status) {
    std::atomic_int success_cnt(0);
    std::vector<bthread_t> bids;
    std::vector<std::unique_ptr<Executor>> execs;
    bids.reserve(searcher_types.size());
    execs.reserve(searcher_types.size());
    for (const std::string &searcher_type : searcher_types) {
        auto it = searchers_.find(searcher_type);
        if (it == searchers_.end()) {
            if (response_status) {
                (*response_status)[searcher_type] =
                        absl::StrFormat("searcher type \"%s\" is invalid!", searcher_type);
            }
            continue;
        }
        auto& searcher_coll = it->second;
        auto& response = (*responses)[searcher_type];
        response.reset(new Response);
        execs.emplace_back(new Executor);
        // NOTE!!! implicit conversion 隐式类型转换
        *(execs.back()) = [&] {
            if (searcher_coll.GetData(request, response.get(),
                        response_status ? &(*response_status)[searcher_type] : nullptr)) {
                ++success_cnt;
            }
        };
        bthread_t bid = 0;
        if (bthread_start_background(&bid, nullptr /* attr */, _BthreadHelperFunc, execs.back().get()) != 0) {
            LOG(ERROR) << "Fail to create bthread";
            continue;
        }
        bids.emplace_back(bid);
    }  // for

    for (bthread_t& bid : bids) {
        bthread_join(bid, nullptr);
    }

    return success_cnt;
}
