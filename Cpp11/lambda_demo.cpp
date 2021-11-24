#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <cstdio>
#include <typeinfo>

using namespace std;

/*
 * About this
 * If a lambda-capture includes a capture-default that is &,
 * the identifiers in the lambda-capture shall not be preceded by &.
 * If a lambda-capture includes a capture-default that is =,
 * the lambda-capture shall not contain  this and each identifier it
 * contains shall be preceded by &. An identifier or this shall not
 * appear more than once in a lambda-capture.
 *
 * So you can use [this], [&], [=] or [&,this] as a lambda-introducer
 * to capture the this pointer by value.
 */
/*
 * void S2::f(int i)
 * {
 *     [&]{}; //ok: by-reference capture default
 *     [=]{}; //ok: by-copy capture default
 *     [&m, n] //ok: m by ref and n by value
 *     [&, i]{}; // ok: by-reference capture, except i is captured by copy
 *     [=, &i]{}; // ok: by-copy capture, except i is captured by reference
 *     [&, &i] {}; // error: by-reference capture when by-reference is the default
 *     [=, this] {}; // error: this when = is the default
 *     [=, *this]{}; // ok: captures the enclosing S2 by copy (C++17)
 *     [i, i] {}; // error: i repeated
 *     [this, *this] {}; // error: "this" repeated (C++17)
 * }
 */
//!! 一般形式: [capture list](param list)->return_type {impl}
// param list and return type are optional, not mandatory
// capture list 只能使用 local non-static 变量，可以使用定义在函数之外的 static 变量
// [&] implicitly capture all by ref; [=] implicitly capture all by value.
// [&, exception_list] 除去指定的，其余的都是引用 [=, &var]

//!! [&os,m] 是capture list，相当于 function object 的成员变量
// x 相当于 operator（） 的形参
// capture list 是针对于lambda之外的本地变量的，可以直接引用的。
// #if 0
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
    auto f = [&] {
        printf( "%d\n", outVar );
    };

    f();

    exit(0);
}

// NOTE!!! cannot capture by non-const ref
// This OK, value is also OK
// int lambda_as_param(const std::function<int(void)> &fn) {
    // return fn();
// }
// FIAL
int lambda_as_param(std::function<int(void)> *fn) {
    return (*fn)();
}

// capture this
struct Foo {
    Foo( const std::string &_Name, int _i, int _j )
        : name(_Name), i(_i), j(_j) {}

    void work()
    {
        auto do_sum = [this]()->int {
            // 在lambda内部可以向用本地变量引用成员变量, 可以直接修改他们
            // 相当于是 [&]
            i *= 2; j *= 2;
            return i + j + name.length();
        };
        // cout << typeid(do_sum).name() << endl;
        // cout << (typeid(do_sum) == typeid(std::function<int(void)>)) << endl;

        cout << do_sum() << endl;
        cout << lambda_as_param(do_sum) << endl;
    }

    int i, j;
    std::string name;
};

void test2()
{
    Foo foo("charles", 3, 2);
    foo.work();
    cout << foo.i << " " << foo.j << endl;

    exit(0);
}

void lambda_capture_alias() {
    struct Foo {
        int x = 0;
    };

    Foo foo;
    // NOTE!!! -std=c++14 required
    auto f = [&val = foo.x]() {
        val = 10;
    };

    f();
    cout << foo.x << endl;
}
// #endif

void thread_with_arg_and_return() {
    std::vector<int> arr{1,2,3,4,5};
    std::vector<int> results(arr.size(), 0);
    std::vector<std::thread> workers(arr.size());
    for (auto i = 0; i < arr.size(); ++i) {
        workers[i] = std::thread([&val = arr[i], &result = results[i]]{
            result = val * val;
        });
    }
    for (auto i = 0; i < arr.size(); ++i) {
        workers[i].join();
    }
    std::copy(results.begin(), results.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
}

int main()
{
    // test();
    // test1();
    // test2();
    // lambda_capture_alias();
    thread_with_arg_and_return();

#if 0
    vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    print_modulo( v1, cout, 3 );

    char *p = NULL;
    [&]{ p = new char[100]; }();
    printf("%lx\n", (long)p);
#endif

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




















