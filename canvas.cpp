/*
 * Canvas: 通用数据容器，一个容器可存储各种数据类型.
 *   namespace_id(int): 用作标识canvas的用途,仅仅是标记而已。可以理解为不同的canvas对象.
 *   redstone提供了一些built-in canvas namespace id (See red_kernel.h), 除了 default_namespace_id=0, 还有
 *   RequestCanvasId=2 主要用于存储请求处理相关的数据如 request, response, controler etc. 由KernelRequest封装，
 *        随每次请求经过的Kernel::Run()而改变;
 *   ResourceCanvasId=1 主要用于存储相对不易变化的辅助资源如feature_client, config etc. 由KernelResource封装。
 *      在AppBundle::AppInit() Kernel::Init() 里初始化
 *
 * Canvas存储的数据类型是一个WrapperClass，里面包裹了原始的数据类型，
 *    这些WrapperClass基类是AbstractSharedType (See canvas/generic.h)
 *    系统提供了一些用于请求处理的WrapperClass: RequestProto, ResponseProto, RpcController etc. (red_kernel.h)
 *
 * Canvas数据存储于成员fields vector中, 数组下标用作标识数据类型的field id
 *   一种数据类型(class)对应一个field id, 对应一个该类型的实例。
 *   即在Canvas中一种数据类型只能有一个实例。
 *
 * Get() 成员函数通过WrapperClass::Id()从数组中找出对应的成员并返回，
 *   若不存在，non-const版本会创建新元素，const版本会返回空指针
 */
template <CanvasNamespaceType namespace_id = default_namespace_id>
class Canvas final {
 public:
    using smart_ptr = std::shared_ptr<void>;
    class FieldData final {
     public:
        FieldData(): data_ptr() { }
        template<typename T>
        explicit FieldData(T* t): data_ptr(t) { }

        template<typename Data>
        void reset(Data* t) {
            if (data_ptr.get()) {
                trash.push_back(data_ptr);
            }
            data_ptr.reset(t);
        }

        template<typename Deletor>
        void reset(void* t, Deletor d) {
            if (data_ptr.get()) {
                trash.push_back(data_ptr);
            }
            data_ptr.reset(t, d);
        }

        void* get() {
            return data_ptr.get();
        }

     private:
        smart_ptr data_ptr;
        std::list<smart_ptr> trash;  // this makes old data not get deleted, for thread-safe consideration
    };

 public:
    // T is canvas wrapper type, T::type is original type
    // 若不存在就创建一个空值并返回
    template <typename T>
    typename T::type* Get() {
        static_assert(T::namespace_id == namespace_id, "namespace_id values are different, check code!!!");
        auto fid = T::Id();  // 一个数据类型(class)对应一个Id, 由RegisterId()生成
        SpinLock::Holder holder(&lock);  // Each request usually has its own Canvas object so it is unlikely to be busy
        if (fid < fields.size()) {
            auto t = static_cast<typename T::type *>(fields[fid].get());
            if (t) {
                return t;
            }
        }
        return UpdateImpl<T>(new typename T::type);
    }

    // NOTE const 版 Get 若找不到返回空指针，不会创建新的
    template <typename T>
    void* Get() const {
        static_assert(T::namespace_id == namespace_id, "namespace_id values are different, check code!!!");
        auto fid = T::Id();
        if (fid < fields.size()) {
            return (const_cast<FieldData&>(fields[fid]).get());
        }
        return nullptr;
    }

    void* Get(size_t fid) {
        SpinLock::Holder holder(&lock);
        if (!IsValidFid(fid)) {
            return nullptr;
        }
        Padding(fid);
        return fields[fid].get();
    }

    template<typename T>
    typename T::type* Update(typename T::type* t) {
        static_assert(T::namespace_id == namespace_id, "namespace_id values are different, check code!!!");
        SpinLock::Holder holder(&lock);
        return UpdateImpl<T>(t);
    }

    template<typename Deletor>
    void* Update(FieldId fid, void* t, Deletor d) {
        SpinLock::Holder holder(&lock);
        return UpdateImpl<Deletor>(fid, t, d);
    }

    // 在全局中，当一个容器第一次被使用时，为这个容器分配一个id。
    static FieldId RegisterId(std::string name) {
        static SpinLock reg_lock;
        SpinLock::Holder holder(&reg_lock);

        FieldId id = field_registry.size();
        field_registry.emplace_back(name);
        LOG(ERROR) << "namespace_id:" <<  namespace_id  << ", RegisterId: name=" << name << ", id=" << id;
        return id;
    }

    std::size_t size() const { return fields.size(); }

 private:
    template<typename T>
    typename T::type* UpdateImpl(typename T::type* t) {
        auto fid = T::Id();
        if (!IsValidFid(fid)) {
            return nullptr;
        }
        Padding(fid);
        fields[fid].reset(t);
        return static_cast<typename T::type *>(fields[fid].get());
    }

    template<typename Deletor>
    void* UpdateImpl(FieldId fid, void* t, Deletor d) {
        if (!IsValidFid(fid)) {
            return nullptr;
        }
        Padding(fid);
        fields[fid].reset(t, d);
        return fields[fid].get();
    }

    bool IsValidFid(FieldId fid) {
        return fid < field_registry.size();
    }

    void Padding(FieldId fid) {
        if (!IsValidFid(fid)) {
            return;
        }
        if (fid >= fields.size()) {
            fields.resize(fid+1);
        }
    }

 private:
    static std::vector<FieldMeta> field_registry;
    std::vector<FieldData> fields;
    SpinLock lock;
};

