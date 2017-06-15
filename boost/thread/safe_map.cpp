// NOTE!!! mutex atomic are nocopyable

template <typename K, typename V>
class SafeMap : public std::map<K, V>
              , public boost::shared_lockable_adapter<boost::shared_mutex> 
{
public:
    typedef typename std::map<K, V>     BaseType;
    typedef SafeMap<K, V>               ThisType;

public:
    // 有copy constructor，必须实现默认constructor
    SafeMap() = default;

    // mutex is nocopyable, 所以必须实现 copy constructor
    SafeMap(const SafeMap<K, V> &other)
    {
        boost::unique_lock<ThisType> wLock(*this);
        // 在模板中，必须显示调用基类函数
        BaseType::clear();
        BaseType::insert(other.begin(), other.end());
    }

    std::pair<typename BaseType::iterator, bool> 
    insert(const typename BaseType::value_type &v)
    {
        boost::unique_lock<ThisType> wLock(*this);
        return BaseType::insert(v);
    }
};

