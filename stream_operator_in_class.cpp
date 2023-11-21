#include <iostream>
#include <sstream>

class Foo {
    // global function declaration in class def
    friend std::ostream& operator<<(std::ostream& os, const Foo &foo);

 public:
    Foo(const std::string& _name, int64_t _id) : name(_name), id(_id) {}
    std::string DebugString() const;

 private:
    std::string name;
    int64_t id = 0;
};

std::string Foo::DebugString() const {
    std::ostringstream oss;
    oss << "{name:" << name << ",id:" << id << "}"; 
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Foo &foo) {
    os << foo.DebugString();
    return os;
}

int main() {
    Foo foo("charles", 12345);
    std::cout << foo << std::endl;

    return 0;
}

#if 0
class FeatureInfo {
 public:
    friend std::ostream& operator<<(std::ostream& os, const FeatureInfo& info) {
        return info.Dump(os);
    }

    std::ostream& Dump(std::ostream& os) const {
        os << id << "\t" << index << "\t" << key << "\t" << name << "\t";
        switch (type) {
            case FeatureType::FT_NUMBER:
            case FeatureType::FT_FLOAT32_LIST:
            case FeatureType::FT_COMMON_NUMBER:
            case FeatureType::FT_COMMON_FLOAT32_LIST: {
                os << float_default_value;
                break;
            }
            case FeatureType::FT_INT64:
            case FeatureType::FT_INT64_LIST:
            case FeatureType::FT_COMMON_INT64:
            case FeatureType::FT_COMMON_INT64_LIST: {
                os << int64_default_value;
                break;
            }
        }
        return os;
    }
};
#endif
