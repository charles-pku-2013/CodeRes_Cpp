#include <map>
#include <string>
#include <iostream>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace std;

typedef std::map<std::string, int>  Dict;

static
void print_dict( const Dict &dict )
{
    for (const auto &v : dict)
        cout << v.first << "\t" << v.second << endl;
}

int main()
{
    Dict dict;
    dict["charles"] = 1;
    dict["pku"] = 2;
    dict["Beijing"] = 3;
    dict["China"] = 4;

    boost::ptr_vector<Dict::value_type, boost::view_clone_allocator> ptrArray(dict.begin(), dict.end());
    ptrArray.sort([](const Dict::value_type &lhs, const Dict::value_type &rhs)->bool {
        return lhs.second > rhs.second;
    });

    //!! 注意，不是想用指针那样，应该看做就是存储的元素
    // for (auto &p : ptrArray)
        // cout << p.first << "\t" << p.second << endl;
    
    for (auto it = ptrArray.begin(); it != ptrArray.end(); ++it)
        cout << it->first << "\t" << it->second << endl;

    cout << "Original dict content:" << endl;
    print_dict(dict);

    return 0;
}
