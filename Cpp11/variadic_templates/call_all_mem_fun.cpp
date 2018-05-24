#include <iostream>
#include <string>
#include <memory>

using namespace std;

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

struct Bar {
    void greet(const std::string &name)
    {
        cout << "Hello " << name << endl;
    }

    void concat(std::string &out, const std::string &s1, const std::string &s2)
    {
        out = s1 + " " + s2;
    }
};

template<typename C, typename F, typename... Args>
void call(C&& obj, F&& func, Args&&... args)
{
    (obj.*func)(std::forward<Args>(args)...);
}

int main()
{
    Foo foo;
    call(foo, &Foo::print_sum, 3, 5);
    call(foo, &Foo::print_data);

    auto pBar = std::make_shared<Bar>();
    call(*pBar, &Bar::greet, "beauty");
    string str;
    call(*pBar, &Bar::concat, str, "hello", "beauty");
    cout << str << endl;

    return 0;
}

