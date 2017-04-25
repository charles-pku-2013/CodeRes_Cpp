/*
 * c++ -o /tmp/test equal_range_is_binary_search.cpp -lglog -std=c++11 -g
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <glog/logging.h>

using namespace std;

struct Foo {
    int n;
    char ch;

    friend std::ostream& operator << (std::ostream &os, const Foo &f)
    {
        os << f.n << ':' << f.ch;
        return os;
    }
};


void test_vec()
{
    vector<Foo> vec;
    vec.reserve(100);
    for (int i = 1; i <= 100; ++i)
        vec.emplace_back(Foo{i, (char)(('A' + i) % ('Z' + 1))});

    struct {
        bool operator()(const Foo &f, int n) const
        { 
            LOG(INFO) << "Cmp1 " << f.n << " and " << n;
            return f.n < n; 
        }
        bool operator()(int n, const Foo &f) const
        { 
            LOG(INFO) << "Cmp2 " << n << " and " << f.n;
            return n < f.n; 
        }
    } cmp;

    int n;
    while (cin >> n) {
        cout << "Looking up with binary_search:" << endl;
        bool found = std::binary_search(vec.begin(), vec.end(), n, cmp);
        cout << (found ? "Found!" : "Not found!") << endl;

        cout << "Looking up with lower_bound:" << endl;
        auto it = std::lower_bound(vec.begin(), vec.end(), n, cmp);
        if (it == vec.end())
            cout << "Not found!" << endl;
        else
            cout << *it << endl;

        cout << "Looking up with equal_range:" << endl;
        auto ret = std::equal_range(vec.begin(), vec.end(), n, cmp);
        if (ret.first == ret.second)
            cout << "Not found!" << endl;
        else
            cout << *ret.first << endl;
    } // while
}


// WRONG!!!
void test_set()
{
    struct Cmp {
        bool operator()(const Foo &f, int n) const
        { 
            LOG(INFO) << "Cmp1 " << f.n << " and " << n;
            return f.n < n; 
        }
        bool operator()(int n, const Foo &f) const
        { 
            LOG(INFO) << "Cmp2 " << n << " and " << f.n;
            return n < f.n; 
        }
    } cmp;

    struct FooCmp {
        bool operator() (const Foo &lhs, const Foo &rhs) const
        { return lhs.n < rhs.n; }  
    };

    set<Foo, FooCmp> set;
    for (int i = 1; i <= 100; ++i)
        set.emplace(Foo{i, (char)(('A' + i) % ('Z' + 1))});

    int n;
    while (cin >> n) {
        cout << "Looking up with binary_search:" << endl;
        bool found = std::binary_search(set.begin(), set.end(), n, cmp);
        cout << (found ? "Found!" : "Not found!") << endl;

        cout << "Looking up with lower_bound:" << endl;
        auto it = std::lower_bound(set.begin(), set.end(), n, cmp);
        if (it == set.end())
            cout << "Not found!" << endl;
        else
            cout << *it << endl;

        cout << "Looking up with equal_range:" << endl;
        auto ret = std::equal_range(set.begin(), set.end(), n, cmp);
        if (ret.first == ret.second)
            cout << "Not found!" << endl;
        else
            cout << *ret.first << endl;
    } // while
}


int main()
{
    // test_vec();
    test_set();

    return 0;
}

