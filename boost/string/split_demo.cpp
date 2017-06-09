#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#define SPACES                " \t\f\r\v\n"

int main()
{
    using namespace std;

    // NOTE!!! 分隔符出现在开头或末尾会导致最后空字符串的产生
    // 相当于分隔符分割的空值null
    // 只要有连续的分割符出现，就会有空字符串(null)产生，而不论是否是同一种分隔符
    // 所以一般在split之前先用分隔符trim
    string line = "123,4,,5:6,"; 
    // boost::trim_if(line, boost::is_any_of(string(",") + SPACES));

    vector<string> values;
    boost::split(values, line, boost::is_any_of(",:")/* , boost::token_compress_on */);
    
    cout << values.size() << endl;
    for (auto &v : values) {
        if (v.empty())
            cout << "null" << endl;
        else
            cout << v << endl;
    } // for

    cout << endl << endl;

    //!! 或者在split之后做以下操作
    auto newEnd = std::remove_if(values.begin(), values.end(), [](const string &s){
        return s.empty();
    });
    values.erase(newEnd, values.end());
    cout << values.size() << endl;

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

