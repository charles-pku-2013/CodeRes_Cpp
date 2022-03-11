// from tensorflow_serving/util/cleanup.h
#include <iostream>
#include <string>
#include <functional>

template <typename F>
class Cleanup {
 public:
    Cleanup() : released_(true), f_() {}

    template <typename G>
    explicit Cleanup(G&& f)          // NOLINT
            : f_(std::forward<G>(f)) {}  // NOLINT(build/c++11)

    Cleanup(Cleanup&& src)  // NOLINT
            : released_(src.is_released()), f_(src.release()) {}

    // Implicitly move-constructible from any compatible Cleanup<G>.
    // The source will be released as if src.release() were called.
    // A moved-from Cleanup can be safely destroyed or reassigned.
    template <typename G>
    Cleanup(Cleanup<G>&& src)  // NOLINT
            : released_(src.is_released()), f_(src.release()) {}

    // Assignment to a Cleanup object behaves like destroying it
    // and making a new one in its place, analogous to unique_ptr
    // semantics.
    Cleanup& operator=(Cleanup&& src) {  // NOLINT
        if (!released_) f_();
        released_ = src.released_;
        f_ = src.release();
        return *this;
    }

    ~Cleanup() {
        if (!released_)
        { f_(); }
    }

    // Releases the cleanup function instead of running it.
    // Hint: use c.release()() to run early.
    F release() {
        released_ = true;
        return std::move(f_);
    }

    bool is_released() const { return released_; }

 private:
    static_assert(!std::is_reference<F>(), "F must not be a reference");

    bool released_ = false;
    F f_;
};

//////////
// Implementation details follow. API users need not read.

template <typename F, typename DecayF = typename std::decay<F>::type>
Cleanup<DecayF> MakeCleanup(F&& f);

template <typename F, typename DecayF>
Cleanup<DecayF> MakeCleanup(F&& f) {
    return Cleanup<DecayF>(std::forward<F>(f));
}

int main()
{
    auto cleanup = MakeCleanup([&]() {
        std::cout << "Cleanning up..." << std::endl;
    });

    // NOTE!!! 手动释放这样做
    cleanup.release()();
    std::cout << "main terminating..." << std::endl;

    return 0;
}
