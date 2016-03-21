#include <iostream>
#include <cstdlib>

using namespace std;

/*
 * 助记：* 左边表示指针要指向的数据类型，如 int, const int, int const;
 * 右边表示指针本身的类型，如是否是const指针，int *const p; 
 * 目前只发现const这一个修饰词。
 */
int main( int argc, char **argv )
{
    int i = 5, j = 7;

    /* case 1 */
    /* "const int" is identical to "int const", qualify data type */
    // const int *p1 = &i;
    // int const *p1 = &i; // above two are identical, a pointer to "const int"

    /* case 2 */
    // 目前看来定义常量指针只有这一种形式
    int *const p1 = &i;    // a pointer itself is const

    // const pointer to const value
    const int *const p1 = &i;
    int const *const p1 = &i;

    // p1 = &j;        // OK with case1, but not with case 2
    // *p1 = 6;     // OK with case 2 but not case 1

    cout << *p1 << endl;

    return 0;
}

