#include <iostream>
#include <map>
#include <string>

#define _ADD_MEMBER(type, name) \
    private: \
        type name##_; \
    public: \
        const type& name() const { return name##_; } \
        void set_##name(const type& val) { name##_ = val; }

using namespace std;

struct Foo {
    using StringMap = std::map<string, string>;
    _ADD_MEMBER(StringMap, dict)
    _ADD_MEMBER(std::string, name)
    _ADD_MEMBER(int64_t, version)
};

int main() {
    Foo foo;
    foo.set_version(123);
    cout << foo.version() << endl;

    return 0;
}
