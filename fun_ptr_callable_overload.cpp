#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

int float_fun(const std::string &str, std::vector<float> *result) {
    cout << "float_fun" << endl;
    return 0;
}

int string_fun(const std::string &str, std::vector<std::string> *result) {
    cout << "string_fun" << endl;
    return 0;
}

struct Callable {
    Callable() = default;
    explicit Callable(void *p) : pfn_(p) {}

    int operator()(const std::string &str, std::vector<std::string> *result) {
        using FunPtr = int(*)(const std::string&, std::vector<std::string>*);
        FunPtr pfn = (FunPtr)pfn_;
        return pfn(str, result);
    }
    int operator()(const std::string &str, std::vector<float> *result) {
        // typedef int(*FunPtr)(const std::string&, std::vector<float>*);
        using FunPtr = int(*)(const std::string&, std::vector<float>*);
        FunPtr pfn = (FunPtr)pfn_;
        return pfn(str, result);
    }

    void *pfn_ = nullptr;
};

int main() {
    std::unordered_map<std::string, Callable> table;
    table["string"] = Callable((void*)&string_fun);
    table["float"] = Callable((void*)&float_fun);

    do {
        std::string s("abc");
        std::vector<std::string> arr;
        auto it = table.find("string");
        (it->second)(s, &arr);
    } while (0);
    do {
        std::string s("abc");
        std::vector<float> arr;
        auto it = table.find("float");
        (it->second)(s, &arr);
    } while (0);

    return 0;
}

