/*
 * https://stackoverflow.com/questions/51791166/stdbind-with-variadic-template-function/51792022#51792022
 */
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

template<typename T>
class ObjFactory {
public:
    typedef std::shared_ptr<T>              ObjPtr;
    typedef std::function<ObjPtr(void)>     CreatorFunc;
public:
    void registerCreator(const std::string &name, const CreatorFunc &creator)
    { m_dictCreator[name] = creator; }

    ObjPtr getObj(const std::string &name) const
    {
        auto it = m_dictCreator.find(name);
        return (it == m_dictCreator.end() ? nullptr : (it->second)());
    }
private:
    std::unordered_map<std::string, CreatorFunc>   m_dictCreator;
};


using namespace std;

struct Base {
    virtual ~Base() {}
    virtual void greet() const
    { cout << "I am Base" << endl; }
};

struct Derived : Base {
    Derived() : x(0), y(0) {}
    Derived(int a, int b) : x(a), y(b) {}
    int x, y;

    virtual void greet() const
    { cout << "I am Derived x = " << x << " y = " << y << endl; }
};

template<typename T, typename... Args>
std::shared_ptr<T> create_obj(Args&&... args)
{ return std::make_shared<T>(std::forward<Args>(args)...); }

int main()
{
    ObjFactory<Base> factory;
    factory.registerCreator("default", std::make_shared<Derived>);
    factory.registerCreator("default1", create_obj<Derived>);
    /*
     * factory.registerCreator("withArgs", std::bind(create_obj<Derived, int, int>, 1, 2)); // WRONG!!! 
     * 强制指定可变模板参数类型，create_obj的形参是int&&，本来是可以接受常数参数如1,2；
     * 但由于bind生成了bind_struct，将常数拷贝到成员变量中，又形成了lvalue，是不能直接传递给rvalue ref形参的。需要std::move
     */

    /*
     * bind成员类型也是const ref，函数形参依据ref collapsing rules，也是const ref
     */
    factory.registerCreator("withArgs", std::bind(create_obj<Derived, const int&, const int&>, 1, 2)); // 要指定详细模板参数列表

    do {
        auto pObj = factory.getObj("default1");
        if (pObj) { pObj->greet(); }
    } while (0);

    do {
        auto pObj = factory.getObj("withArgs");
        if (pObj) { pObj->greet(); }
    } while (0);

    return 0;
}

