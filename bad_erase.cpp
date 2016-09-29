#include <list>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

template<typename T>
void print_container(const T &c)
{
    std::copy(c.begin(), c.end(), std::ostream_iterator<typename T::value_type>(cout, " "));
    cout << endl;
}

int main()
{
    list<int> l1, l2;
    l1.push_back(1);
    l1.push_back(2);
    l2.push_back(3);
    l2.push_back(4);

    list<int>::iterator it = l1.begin();
    l2.erase(it);

    print_container(l1);
    print_container(l2);

    return 0;
}

