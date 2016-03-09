#include <iostream>


template <char *p>
struct foo
{
    void bar();
};

static char a ='a';

namespace
{
    char b = 'b';
    static char c = 'c';

    template <class T> struct xxx {};

    void foobar()
    {
        struct no_linkage {};
        xxx<no_linkage>(); // 如果编译错误，说明no_linkage的linkage没有变化
    }
}

int main()
{
    foo<&a>().bar(); // 由于a的linkage是internal，因此应该编译错误
    foo<&b>().bar(); // 如果编译正确，说明b的linkage是external
    foo<&c>().bar(); // 如果编译错误，说明c的linkage是internal

    foobar();

    return 0;
}
