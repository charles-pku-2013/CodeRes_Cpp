#include <cstdio>


class Demo {
public:
    static const int count/* = 5*/;
    static const int *p/* = new int(5)*/;   //!! WRONG, 必须在外初始化
};

const int Demo::count = 5; //!! 要想使用其地址，line18，必须这样声明和定义
const int *Demo::p = new int(5);

int main()
{
//    Demo d;
    printf( "%d\n", Demo::count );
    const int *p = &Demo::count;
    
    return 0;
}


















