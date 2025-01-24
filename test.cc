#if 0
  2. 给你N个实例ip，用C++实现一个轮询的负载均衡算法，每次返回一个ip，必须满足以下三个要求
    1. 严格轮询（必须严格按照0..N-1,0..N-1顺序返回）
    2. 线程安全
    3. 高性能
#endif
 

#include <vector>
#include <string>
#include <mutex>
#include <atomic>


class IpPool {
public:
    explicit IpPool(std::size_t n) {
        pool_.reserve(n);
    }

    const std::string& getIp() {
        // std::shared_lock lck(mtx_);
        // idx_ = (idx + 1) % pool_.size();
        // return ip;
        return pool_[idx_++ % pool_.size()];
    }

private:
    // boost::cycle_queue<>   
    IpPool pool(5);
    std::vector<std::string> pool_;
    std::atomic_size_t idx_{0};
    // std::shared_mutex  mtx_;
};
