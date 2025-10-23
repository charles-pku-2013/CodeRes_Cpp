template<typename T>
void func(T&& param); // T需要被推导，所以 T&& 是万能引用

template<typename T>
class MyClass {
    void push_back(T&& param); // T在类实例化时已确定，因此 T&& 是普通的右值引用
};
