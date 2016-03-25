#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

//!! 一般形式: [capture list](param list)->return type {impl}
// param list and return type are optional, not mandatory
// capture list 只能使用 local non-static 变量，可以使用定义在函数之外的 static 变量
// [&] implicitly capture all by ref; [=] implicitly capture all by value.
// [&, exception_list] 除去指定的，其余的都是引用 [=, &var]

//!! [&os,m] 是capture list，相当于 function object 的成员变量
// x 相当于 operator（） 的形参
// capture list 是针对于lambda之外的本地变量的，可以直接引用的。
void print_modulo(const vector<int> &v, ostream& os, int m)
// output v[i] to os if v[i]%m==0
{
    // for_each(begin(v), end(v),
        // [&os, m](int x) { if(x % m == 0) os << x << '\n'; }
    // );
    for_each(begin(v), end(v),
        [&](int x) { if(x % m == 0) os << x << '\n'; }
    );
}


void test()
{
    char *p = NULL;
    /*
     * variable 'p' cannot be implicitly captured in a lambda with no capture-default specified
     * 必须指定 capture 方式：& 或是 =
     * capture list 中的变量是在lambda定义的时候完成赋值的。
     * 本例中若为=，则在lambda定义时候，p还是空指针，被复制到capture list中，
     * 之后p的变化不会反馈到capture list中，但如果是 &，由于capture list中存储的是p的引用，
     * 所以之后p的变化会反馈到capture list中。
     */
    // lambda若指定返回值类型必须有参数列表的()
    // lambda requires '()' before return type
    auto print = [&]()->int {
        if( p )
            printf( "%s\n", p ); // &
        else
            printf( "p is empty pointer.\n" ); // =
        return 105;
    };

    p = new char[100];
    sprintf(p, "Hello, lambda.");
    int ret = print();
    printf( "%d\n", ret );

    exit(0);
}

void test1()
{
    int outVar = 10;

    // 不指定capture方式默认不capture
    auto f = [] {
        printf( "%d\n", outVar );  
    };

    f();

    exit(0);
}


int main()
{
    // test();
    test1();

    vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    print_modulo( v1, cout, 3 );
	
	char *p = NULL;
	[&]{ p = new char[100]; }();
	printf("%lx\n", (long)p);
	
    return 0;
}


/* 
void algo(vector<int>& v)
{
    sort(v.begin(),v.end()); // sor t values
    // ...
    sort(v.begin(),v.end(),[](int x, int y) { return abs(x)<abs(y); }); // sor t absolute values
    // ...
}
 */

/* 
void f(vector<int>& v)
{
    bool sensitive = true;
    // ...
    sort(v.begin(),v.end(),
        [](int x, int y) { return sensitive ? x<y : abs(x)<abs(y); } //!! error : can’t access sensitive
    );
}
 
void f(vector<int>& v)
{
    bool sensitive = true;
    sort(v.begin(),v.end()
        [sensitive](int x, int y) { return sensitive ? x<y : abs(x)<abs(y); }
    );
    //!! By mentioning sensitive in the capture list, we make it accessible from within the lambda
}
*/






 
 
 
 
 
 
 
 
 
 
 
 
 
 
