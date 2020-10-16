#include <iostream>
#include <memory>
#include <cassert>

class WrapperBase {
public:
    virtual ~WrapperBase() = default;
    virtual void* get_raw() = 0;

    // template<typename T>
    // T* get_type()
    // { return dynamic_cast<T*>(static_cast<T*>(get_raw())); }
};

template<typename T>
class Wrapper : public WrapperBase {
public:
    Wrapper(std::shared_ptr<T> pobj) : obj_(std::move(pobj)) {}
    T* get() { return obj_.get(); }
    void* get_raw() override { return get(); }
private:
    std::shared_ptr<T> obj_;
};

struct Foo {
    Foo()
    { std::cout << "Foo constructor" << std::endl; }
    ~Foo()
    { std::cout << "Foo destructor" << std::endl; }
    void greet() const
    { std::cout << "Hello Foo" << std::endl; }
};

int main() {
    std::shared_ptr<WrapperBase> p1(new Wrapper<Foo>(std::make_shared<Foo>()));

    // Foo* pfoo = p1->get_type<Foo>();
    // if (pfoo) { pfoo->greet(); }

    // given type Foo
    do {
        Wrapper<Foo>* wrapper = dynamic_cast<Wrapper<Foo>*>(p1.get());
        assert(wrapper);
        Foo *pfoo = wrapper->get();
        assert(pfoo);
        pfoo->greet();
    } while (0);

    return 0;
}


