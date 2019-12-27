#include <iostream>
#include <vector>
#include <map>

using namespace std;

template<typename T>
struct Printer {
    void operator()(const T &val)
    { cout << val << endl; }
};

template<typename T>
struct Printer<std::vector<T>> {
    void operator()(const std::vector<T> &val) {
        for (auto &v : val) {
            cout << v << " ";
        }
        cout << endl;
    }
};

template<typename T>
void print(const T &val)
{ cout << val << endl; }

template<typename K, typename V>
void print(const std::map<K,V> &val) { 
    for (auto &kv : val) {
        cout << kv.first << " = " << kv.second << endl;
    }
    cout << endl;
}

int main() {
    // Printer<int> printer;
    // printer(5);

    // vector<int> v{1,2,3,4,5};
    // Printer<std::vector<int>> printer1;
    // printer1(v);
    
    // print(5);
    // print(v);
    
    map<string, int> dict{{"Beijing", 1}, {"Shanghai", 2}, {"Shenzhen", 3}};
    print(dict);

    return 0;
}

