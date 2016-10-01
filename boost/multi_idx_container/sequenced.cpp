#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

// NOTE!!! namespace alias
namespace MultiIdx = boost::multi_index;

// The identity key extractor returns the entire base object as the associated key.
// identity 返回整个对象
// define a multi_index_container with a list-like index and an ordered index
typedef MultiIdx::multi_index_container <
    std::string,
    MultiIdx::indexed_by <
        MultiIdx::sequenced<>, // list-like index
        MultiIdx::ordered_non_unique< MultiIdx::identity<std::string> > // words by alphabetical order
    >
> TextContainer;


int main()
{
    using namespace std;

    string text = "Great John once said white horse is not a horse";

    TextContainer tc;
    stringstream stream(text);
    std::copy(istream_iterator<string>(stream), istream_iterator<string>(), back_inserter(tc));

    TextContainer::nth_index<0>::type& seqIdx = tc.get<0>();

    TextContainer::nth_index<1>::type& sortedIdx = tc.get<1>();
    cout << sortedIdx.count("horse") << endl;

    std::copy(seqIdx.begin(), seqIdx.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    sortedIdx.erase("horse");

    std::copy(seqIdx.begin(), seqIdx.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    return 0;
}
