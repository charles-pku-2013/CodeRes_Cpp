
class DegraderManager {
public:
    DegraderManager() {}

    template <class Deg, class... Args>
    void setupDegrader(const std::string& name, Args&&... args) {
        if (degraders_tmp_.find(name) == degraders_tmp_.end()) {
            degraders_tmp_[name].reset(new Deg());
        }
        Degrader* d = degraders_tmp_[name].get();
        Deg* deg = dynamic_cast<Deg*>(d);
        if (deg) {
            deg->setup(std::forward<Args>(args)...);
        }
    }

    void changeDegrader() {
        {
            WLockGuard guard(lock_);
            degraders_ = degraders_tmp_;
        }

        degraders_tmp_.clear();
    }

    bool hit(const std::string& name) {
        RLockGuard guard(lock_);
        auto it = degraders_.find(name);
        if (it != degraders_.end()) {
            return it->second.get()->needDemote();
        }
        return false;
    }

    template<typename T>
    bool get(const std::string& name, T* val) {
        assert(val);
        RLockGuard guard(lock_);
        auto it = degraders_.find(name);
        if (it != degraders_.end()) {
            return it->second.get()->getDemote(val);
        }
        return false;
    }

private:
    std::map<std::string, std::shared_ptr<Degrader>> degraders_;
    std::map<std::string, std::shared_ptr<Degrader>> degraders_tmp_;
    RWLock lock_;
};
