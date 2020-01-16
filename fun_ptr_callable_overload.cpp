#include <iostream>
#include <vector>
#include <functional>

using namespace std;

int test_fun(const std::string &str, std::vector<float> *result) {
    cout << "test_fun" << endl;
    return 0;
}

struct Foo {
    int operator()(const std::string &str, std::vector<std::string> *result) {
        return 0;
    }
    int operator()(const std::string &str, std::vector<float> *result) {
        // typedef int(*FunPtr)(const std::string&, std::vector<float>*);
        using FunPtr = int(*)(const std::string&, std::vector<float>*);
        FunPtr pfn = &test_fun;
        return pfn(str, result);
    }

    void *func = nullptr;
};

int main() {

    string s("abc def");
    Foo foo;
    std::vector<float> arr;
    foo(s, &arr);

    return 0;
}

