#include <iostream>
#include <vector>
#include <boost/format.hpp>

using namespace std;

int main()
{
    vector<int> a;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;
    a.push_back(1);
    cout << boost::format("%lx %lu") % &a[0] % a.capacity() << endl;

    return 0;
}

