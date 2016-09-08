#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <set>
#include <vector>
#include <deque>
#include <iostream>
#include <cassert>

template<typename Iter>
void test(Iter beg, Iter end)
{
    using namespace std;

    typedef typename Iter::value_type DataType;

    for (; beg != end; ++beg) {
        std::shared_ptr<DataType> ptr(
            const_cast<DataType*>(&(*beg)),
            [](DataType*){}
        );
        cout << *ptr << endl;
    } // for
}

int main()
{
    using namespace std;
    test( istream_iterator<string>(cin), istream_iterator<string>() );

    return 0;
}


