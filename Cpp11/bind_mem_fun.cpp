#include <functional>
#include <iostream>

using namespace std;

struct Foo {
    Foo() : m_nData(0) {}

    void add(int n)
    { m_nData += n; }

    int m_nData;
};

int main()
{
    Foo a;
    // auto f = std::bind(&Foo::add, &a, 5);   // right 5 Recommended
    // auto f = std::bind(&Foo::add, a, 5);   // WRONG 0
    auto f = std::bind(&Foo::add, std::ref(a), 5);   // right 5
    auto g = std::bind(&Foo::add, std::ref(a), std::placeholders::_1);   // right 5
    f();
    g(5);

    cout << a.m_nData << endl;

	return 0;
}

// NOTE!!! 绑定成员函数占位符不可以省略
#if 0
class TfsHostSet final : public boost::shared_lockable_adapter<boost::shared_mutex> {
 public:
    using ReadZkEventProcessor = jdsearch::zookeeper::ReadZkEventProcessor;

 public:
    static TfsHostSet& Instance() {
        static TfsHostSet inst;
        return inst;
    }

    bool Init() {
        if (event_processor_) { return true; }  // already inited
        event_processor_.reset(new ReadZkEventProcessor(ZK_HOST(), ZK_NODE(), ZK_USER(), ZK_PASS()));
        int ctx = 0;
        bool ret = event_processor_->Init(std::bind(&TfsHostSet::_OnNodeChange, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), &ctx);
        if (!ret) { event_processor_.reset(); }
        return ret;
    }

 private:
    void _OnNodeChange(std::string path, std::string value,
                 std::map<std::string, std::string> children, void *ctx) {
        // std::cout << "read watcher:---------------"
                // << "path: " << path << ", value: " << value << ", this: " << ctx
                // << std::endl;
        // for (const auto &a : children) {
            // std::cout << a.first << ":" << a.second << std::endl;
        // }
    }

    TfsHostSet() = default;
    TfsHostSet(const TfsHostSet&) = delete;
    TfsHostSet& operator=(const TfsHostSet&) = delete;

    static constexpr const char* ZK_HOST() { return "11.20.213.35:2181"; }
    static constexpr const char* ZK_NODE() { return "/tfs/proxy/lf_online"; }
    static constexpr const char* ZK_USER() { return "digest"; }
    static constexpr const char* ZK_PASS() { return "jd:jd618"; }

    std::unique_ptr<ReadZkEventProcessor> event_processor_;
};
#endif
