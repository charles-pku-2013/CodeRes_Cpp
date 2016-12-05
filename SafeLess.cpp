#include <iostream>

using namespace std;

/*
 * NOTE!!! singed unsigned 比较，自动将signed转换成unsigned
 */


/*
 * 先要假设unsigned都是 >= 0
 */

inline
bool SafeLess( int x, unsigned int y )
{ return (x < 0) || (static_cast<unsigned int>(x) < y); }

inline
bool SafeLess( unsigned int x, int y )
{ return (y >= 0) && (x < static_cast<unsigned int>(y)); }


int main()
{
    int i = -5;
    std::size_t sz = 100;
    uint32_t j = 0;

    // if (i < sz)
        // cout << "less" << endl;
    // else
        // cout << "greater" << endl;

    // if (i < j)
        // cout << "blow zero" << endl;
    // else if (i > j)
        // cout << "above zero" << endl;

    if (SafeLess(i, sz))
        cout << "less" << endl;
    else
        cout << "greater" << endl;

    if (SafeLess(i, j))
        cout << "blow zero" << endl;
    else if (SafeLess(j, i))
        cout << "above zero" << endl;

    return 0;
}

