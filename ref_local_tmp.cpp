#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

struct Foo {
    const std::string& lookup(const std::string &key)
    {
        // static const std::string _fail = ""; // solution
        auto it = dict_.find(key);
        if (it != dict_.end())
            return it->second;
        return ""; // warning: returning reference to local temporary object
    }

    std::unordered_map<std::string, std::string>    dict_;
};

int main()
{
    Foo foo;
    const std::string &result = foo.lookup("abc");
    cout << result << endl;

    return 0;
}
