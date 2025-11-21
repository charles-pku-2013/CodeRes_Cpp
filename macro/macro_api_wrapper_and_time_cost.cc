#define ZEBRA_API(func_name, ...)                                                               \
    do {                                                                                        \
        int err = func_name(__VA_ARGS__);                                                       \
        if (err) {                                                                              \
            throw std::runtime_error(                                                           \
                fmt::format(#func_name " fail: {}", vaisw_embd_error::exception_message(err))); \
        }                                                                                       \
    } while (0)

#define ZEBRA_API_DEBUG(item, func_name, ...)                                             \
    do {                                                                                  \
        auto start_tp = std::chrono::high_resolution_clock::now();                        \
        ZEBRA_API(func_name, __VA_ARGS__);                                                \
        auto                          end_tp = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> elapsed(end_tp - start_tp);                         \
        DLOG_IF(WARNING, elapsed.count() > 1.0) << fmt::format(                           \
            "Item {} call {} time cost too long {}", item, #func_name, elapsed.count());  \
    } while (0)
