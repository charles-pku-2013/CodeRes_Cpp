#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

template <typename Iterator>
void FooImpl(Iterator first, Iterator last, std::random_access_iterator_tag) { 
    cout << "Random access iterator" << endl;
}

template <typename Iterator>
void Foo(Iterator first, Iterator last) {
    typedef typename std::iterator_traits<Iterator>::iterator_category category;
    return FooImpl(first, last, category());
}

int main() {
    vector<int> arr{5,1,6,2,4};
    Foo(arr.begin(), arr.end());

    list<int> l(arr.begin(), arr.end());
    Foo(l.begin(), l.end());

    return 0;
}

