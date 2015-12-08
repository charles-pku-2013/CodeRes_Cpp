#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//!! 一般形式: [capture list](param list)->return type {impl}
// param list and return type are optional, not mandatory
// capture list 只能使用 local non-static 变量，可以使用定义在函数之外的 static 变量
// [&] implicitly capture all by ref; [=] implicitly capture all by value.
// [&, exception_list] 除去指定的，其余的都是引用 [=, &var]

//!! [&os,m] 是capture list，相当于 function object 的成员变量
// x 相当于 operator（） 的形参
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


int main()
{
    vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    print_modulo( v1, cout, 3 );
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






 
 
 
 
 
 
 
 
 
 
 
 
 
 
