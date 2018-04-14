void func(type1 val1, type2 val2)

type1 local1;
std::bind(func, std::ref(local1), _1);

template<type1> reference_wrapper<type1>
struct BindObj {
    BindObj(const type1 &_Val1) : _val1(_Val1) {}

    operator()(const type2 &val2)
    { func(_val1, val2); }

    type1 _val1;
};

