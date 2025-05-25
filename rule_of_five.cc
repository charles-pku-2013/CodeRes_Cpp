/*
https://en.cppreference.com/w/cpp/language/rule_of_three
Rule of five
Because the presence of a user-defined (include = default or = delete declared) destructor,
copy-constructor, or copy-assignment operator prevents implicit definition of the move constructor
and the move assignment operator, any class for which move semantics are desirable, has to declare
all five special member functions:
 */

class rule_of_five {
    char* cstring;  // raw pointer used as a handle to a
                    // dynamically-allocated memory block
 public:
    explicit rule_of_five(const char* s = "") : cstring(nullptr) {
        if (s) {
            cstring = new char[std::strlen(s) + 1];  // allocate
            std::strcpy(cstring, s);                 // populate
        }
    }

    ~rule_of_five() {
        delete[] cstring;  // deallocate
    }

    rule_of_five(const rule_of_five& other)  // copy constructor
        : rule_of_five(other.cstring) {}

    rule_of_five(rule_of_five&& other) noexcept  // move constructor
        : cstring(std::exchange(other.cstring, nullptr)) {}

    rule_of_five& operator=(const rule_of_five& other)  // copy assignment
    {
        // implemented as move-assignment from a temporary copy for brevity
        // note that this prevents potential storage reuse
        return *this = rule_of_five(other);
    }

    rule_of_five& operator=(rule_of_five&& other) noexcept  // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }

    // alternatively, replace both assignment operators with copy-and-swap
    // implementation, which also fails to reuse storage in copy-assignment.
    //  rule_of_five& operator=(rule_of_five other) noexcept
    //  {
    //      std::swap(cstring, other.cstring);
    //      return *this;
    //  }
};
