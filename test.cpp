#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <functional>

using namespace std;

typedef std::pair<int, int> IntPair;

struct Base : std::function<bool(const IntPair&, const IntPair&)> {
    virtual bool operator()(const IntPair &lhs, const IntPair &rhs) const = 0;
    // { cout << "Base op" << endl; }
};

struct Derived : Base {
    // virtual void operator()()
    // { cout << "Derived op" << endl; }

    virtual bool operator()(const IntPair &lhs, const IntPair &rhs) const
    {
        if (lhs.first != rhs.first) {
            return lhs.first > rhs.first;
        }
        return lhs.second < rhs.second;
    }
};

int main()
{
    Base *p = new Derived;
    // (*p)();
    
    std::vector<IntPair> vec{{1,1}, {2,3}, {3,2}, {4,5}};
    std::sort(vec.begin(), vec.end(), std::ref(*p));
    // std::sort(vec.begin(), vec.end(), *p);

    return 0;
}
