#include <random>
#include <iostream>
#include <memory>
#include <functional>
#include <thread>
 
struct Foo {
    void print_sum(int n1, int n2)
    {
        std::cout << n1+n2 << '\n';
    }

    void print_data()
    {
        std::cout << data << std::endl;
    }

    int data = 10;
};

/*
 * NOTE!!! 成员函数指针和bind成员函数的区别
 * bind成员函数需在编译时确定用那个对象调用成员函数。
 * 成员函数指针可以在运行时runtime确定通过哪个对象调用成员函数
 */
void mem_fun_demo()
{
    //  格式 typedef RetType (Classname::*T)(arglist);
    typedef void (Foo::*PrintSum)(int, int);
    // auto pfun = &Foo::print_sum;
    PrintSum pfun = &Foo::print_sum;

    Foo foo;
    (foo.*pfun)(3, 5);

    Foo *ptr = &foo;
    (ptr->*pfun)(5, 7);
}
 
int main()
{
    mem_fun_demo();
    exit(0);

    using namespace std::placeholders;  // for _1, _2, _3...
 
    // bind to a member function
    Foo foo;
    // auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    // f3(5);

    // auto f = std::bind(&Foo::print_data, &foo);
    // f();
    
    // 仅和对象绑定，不改变参数
    auto f = std::bind(&Foo::print_sum, &foo, _1, _2);
    f(20, 30);
    
    std::thread thr1( std::bind(&Foo::print_data, &foo) );
    thr1.join();
 
    std::thread thr2( std::bind(&Foo::print_sum, &foo, 95, _1), 5 );
    thr2.join();

    //!! The delete is replaced and this will lead to mem leak
    // Foo *ptr = NULL;
    // std::shared_ptr<Foo> pFoo( (ptr = new Foo), std::bind(&Foo::print_data, ptr) );
    
	//!! 不严谨！eleter must be callable for the type Y, i.e. d(ptr) must be well formed, 
    // std::shared_ptr<Foo> pFoo( &foo, std::bind(&Foo::print_data, &foo) );
	std::shared_ptr<Foo> pFoo( &foo, [](Foo *p){ p->print_data(); } );
    pFoo->print_sum( 10, 20 );

    return 0;
}


// example 1
#if 0
void do_request(const std::vector<LogReq::pointer> &vecReq,
                std::vector<QsRes::pointer> &vecRes)
{
    typedef void (MTSearchClient::*QsApi)(SearchMultiRes&, const SearchQueryReq&);
    static std::unordered_map<std::string, QsApi> dictApi = {
        {"MTSearchDeal", &MTSearchClient::MTSearchDeal},
        {"MTSearchPoi", &MTSearchClient::MTSearchPoi},
        {"MTMultiSearch", &MTSearchClient::MTMultiSearch},
        {"MTMultiSearchDealPoi", &MTSearchClient::MTMultiSearchDealPoi},
        {"AttributeFilter", &MTSearchClient::AttributeFilter}
    };

    cerr << "Requesting " << g_strHost << ":" << g_nPort << endl;

    vecRes.clear();

    ThriftClient<MTSearchClient>    thriftCli(g_strHost, (uint16_t)g_nPort);
    THROW_RUNTIME_ERROR_IF(!thriftCli.start(10), "Connect to remote server fail!");

    auto pClient = thriftCli.client().get();
    for (auto it = vecReq.begin(); it != vecReq.end(); ++it) {
        auto apiIt = dictApi.find((*it)->ifName());
        if (apiIt == dictApi.end()) {
            LOG(ERROR) << (*it)->ifName() << " is not a valid ifname! LogNO = " << (*it)->logNO();
            continue;
        } // if
        auto pRes = std::make_shared<QsRes>();
        try {
            (pClient->*(apiIt->second))(pRes->res(), (*it)->query()); // NOTE!!! call with memfun ptr
        } catch (const std::exception &ex) {
            LOG(ERROR) << "RPC request error: " << ex.what();
            continue;
        } catch (...) {
            LOG(ERROR) << "RPC request error!";
            continue;
        } // try
        pRes->setNO((*it)->logNO());
        vecRes.push_back(pRes);
    } // for it
}
#endif

// example 2
#if 0
class LogReq {
public:
    typedef std::shared_ptr<LogReq>     pointer;

public:
    typedef void (SearchQueryReq::*StringSetter)(const std::string&);
    typedef void (SearchQueryReq::*IntSetter)(const int32_t);
    typedef void (SearchQueryReq::*DictSetter)(const std::map<std::string, std::string>&);

public:
    static bool init()
    {
        m_setValidIfName = {"", "MTSearchDeal", "MTSearchPoi", "MTMultiSearch", "MTMultiSearchDealPoi", "AttributeFilter"};

        m_mapSearchReqSetterString["query"] = &SearchQueryReq::__set_key_words;
        m_mapSearchReqSetterString["key_words"] = &SearchQueryReq::__set_key_words; // for read json
        m_mapSearchReqSetterString["city"] = &SearchQueryReq::__set_city;
        m_mapSearchReqSetterString["category"] = &SearchQueryReq::__set_category;
        m_mapSearchReqSetterString["location"] = &SearchQueryReq::__set_location;
        m_mapSearchReqSetterString["orderby"] = &SearchQueryReq::__set_orderby;

        m_mapSearchReqSetterInt["cityid"] = &SearchQueryReq::__set_cityid;
        m_mapSearchReqSetterInt["reqid"] = &SearchQueryReq::__set_id;
        m_mapSearchReqSetterInt["id"] = &SearchQueryReq::__set_id; // for read json
        m_mapSearchReqSetterInt["offset"] = &SearchQueryReq::__set_offset;
        m_mapSearchReqSetterInt["limit"] = &SearchQueryReq::__set_limit;
        m_mapSearchReqSetterInt["opt"] = &SearchQueryReq::__set_opt;

        m_mapSearchReqSetterDict["filter"] = &SearchQueryReq::__set_filter;
        m_mapSearchReqSetterDict["counter"] = &SearchQueryReq::__set_counter;
        m_mapSearchReqSetterDict["control"] = &SearchQueryReq::__set_control;
        m_mapSearchReqSetterDict["ext_data"] = &SearchQueryReq::__set_exdata;
        m_mapSearchReqSetterDict["exdata"] = &SearchQueryReq::__set_exdata; // for read json

        return true;
    }

    LogReq() : m_nNO(0) {}

    template<typename T>
    void setAttr(const std::string &key, const T &val)
    {
        if (!setSearchQueryReq(key, val))
            setAttrHelper(key, val);
    }

    void toJson(std::string &out, bool concise = false) const;
    std::string toJson(bool concise = false) const;
    void toJson(Json::Value &root) const;

    void fromJson(const Json::Value &js);

    static void SearchQueryReq2Json(Json::Value &root, const SearchQueryReq &req);

public:
    bool setSearchQueryReq(const std::string &key, const std::string &val)
    {
        auto it = m_mapSearchReqSetterString.find(key);
        if (it == m_mapSearchReqSetterString.end())
            return false;
        (m_SearchQueryReq.*(it->second))(val);
        return true;
    }

    bool setSearchQueryReq(const std::string &key, const int32_t val)
    {
        auto it = m_mapSearchReqSetterInt.find(key);
        if (it == m_mapSearchReqSetterInt.end())
            return false;
        (m_SearchQueryReq.*(it->second))(val);
        return true;
    }

    bool setSearchQueryReq(const std::string &key, const std::map<std::string, std::string> &val)
    {
        auto it = m_mapSearchReqSetterDict.find(key);
        if (it == m_mapSearchReqSetterDict.end())
            return false;
        (m_SearchQueryReq.*(it->second))(val);
        return true;
    }

    // 除了string之外，都是空实现
    template<typename T>
    void setAttrHelper(const std::string &key, const T &val) {}

    // 特化模板，string类型特殊处理
    void setAttrHelper(const std::string &key, const std::string &val)
    {
        if ("if_name" == key) {
            THROW_RUNTIME_ERROR_IF(!m_setValidIfName.count(val), val << "is not a valid if_name");
            m_strIfName = val;
        } // if
    }

    SearchQueryReq& query() { return m_SearchQueryReq; }
    const SearchQueryReq& query() const { return m_SearchQueryReq; }

    const std::string& ifName() const { return m_strIfName; }

    void setNO(std::size_t no) {m_nNO = no;}
    std::size_t logNO() const { return m_nNO; }

private:
    std::size_t              m_nNO;
    // 独有的成员
    std::string              m_strIfName;
    SearchQueryReq           m_SearchQueryReq;

    static std::set<std::string>    m_setValidIfName;
    // 随SearchQueryReq而变
    static std::map<std::string, StringSetter> m_mapSearchReqSetterString;
    static std::map<std::string, IntSetter>    m_mapSearchReqSetterInt;
    static std::map<std::string, DictSetter>   m_mapSearchReqSetterDict;
};
#endif
