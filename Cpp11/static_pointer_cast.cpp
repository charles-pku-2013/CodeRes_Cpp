#include <iostream>
#include <memory>
 
struct BaseClass {};
 
struct DerivedClass : BaseClass
{
    void f() const
    {
        std::cout << "Hello World!\n";
    }
};
 
int main()
{
    std::shared_ptr<BaseClass> ptr_to_base(std::make_shared<DerivedClass>());
 
    // ptr_to_base->f(); // Error won't compile: BaseClass has no member named 'f'
 
    std::static_pointer_cast<DerivedClass>(ptr_to_base)->f(); // OK
    // (constructs a temporary shared_ptr, then calls operator->)
 
    static_cast<DerivedClass*>(ptr_to_base.get())->f(); // also OK
    // (direct cast, does not construct a temporary shared_ptr)
    
    return 0;
}
