#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>


int main()
{
    using namespace std;

    // 分隔符出现在开头或末尾会导致最后空字符串的产生
    // 所以一般在split之前先trim
    string line = "123,4,,5,6,"; 
    vector<string> values;
    boost::split(values, line, boost::is_any_of(","), boost::token_compress_on);
    
    cout << values.size() << endl;
    for (auto &v : values) {
        if (v.empty())
            cout << "null" << endl;
        else
            cout << v << endl;
    } // for

    return 0;
}

#if 0
5
123
4
5
6
null
#endif

