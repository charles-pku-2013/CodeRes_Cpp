#include <iostream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

struct Foo {
    int n;
    double f;
    string info;
};

ostream& operator << (ostream &os, const Foo &f)
{
    os << f.n << " " << f.f << " " << f.info;
    return os;
}

void test1()
{
    vector<int> v = {1,2,3,4,5};
    cout << v.size() << endl;
}

void test2()
{
    vector<Foo> v;
    v.emplace_back( Foo{1, 1.2, "one"} );
    v.emplace_back( Foo{2, 2.3, "two"} );
    v.emplace_back( Foo{3, 3.4, "three"} );

    copy( v.begin(), v.end(), ostream_iterator<Foo>(cout, "\n") );
}

int main()
{
    // test1();
    test2();

    return 0;
}

