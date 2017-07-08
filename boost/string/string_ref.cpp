#include <iostream>
#include <memory>
#include <map>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/utility/string_ref.hpp>

/*
 * 函数形参应尽可能使用string_ref(不需要引用, 也不需要const修饰)
 */

using namespace std;

typedef boost::string_ref   string_ref_type;


boost::string_ref extract_part(boost::string_ref bar)
{ return bar.substr( 2, 3 ); }


// NOTE!!! 🔴🔴 不存在string_ref 到 (const) std::string& 的隐式类型转换, 但反之可以
void print_len(const std::string &s)
{ cout << "length of \"" << s << "\" is " << s.length() << endl; }

void substr_test()
{
    string str = "key:value";
    auto idx = str.find_first_of(':');
    string_ref_type refKey(str.c_str(), idx);
    string_ref_type refVal(str.c_str() + idx + 1);
    // boost::trim(refVal);     // ERROR!!! 🔴🔴
    // refVal[0] = 'h';     // ERROR!!! 🔴🔴  string_ref 是常亮，不可以通过其改变所指字符串内容

    // WRONG!!
    // print_len(refKey);
    // print_len(refVal);

    // CORRECT 🔴🔴 需要显示类型构造
    print_len(string(refKey));
    print_len(string(refVal));

    cout << refKey << endl;
    cout << refVal << endl;
}


struct Employee {
    const std::string     name;
    int             age;
    double          salary;

    friend std::ostream& operator << (std::ostream &os, const Employee &ee)
    {
        os << boost::format("[Employee: %s, age = %d, salary = %lf]") % ee.name % ee.age % ee.salary;
        return os;
    }
};

void map_key_test()
{
    typedef std::shared_ptr<Employee>   EmployeePtr;
    typedef std::map<string_ref_type, EmployeePtr>  EmployeeTable;

    EmployeeTable table;
    EmployeePtr  p;
    p = std::make_shared<Employee>(Employee{"Lucy", 25, 100.5});
    table.emplace(std::make_pair(p->name, p));
    p = std::make_shared<Employee>(Employee{"Lucy", 30, 25.6});
    table.emplace(std::make_pair(p->name, p));
    p = std::make_shared<Employee>(Employee{"Charles", 33, 100});
    table.emplace(std::make_pair(p->name, p));
    p = std::make_shared<Employee>(Employee{"John", 53, 200});
    table.emplace(std::make_pair(p->name, p));

    for (auto &kv : table)
        cout << kv.first << ": " << *(kv.second) << endl;
    cout << endl;

    // string_ref 可以接受 const char* 也可以接受 std::string
    auto lookup = [&](string_ref_type name) {
        auto it = table.find(name);
        if (it == table.end()) {
            cout << "Not Found!" << endl;
        } else {
            cout << *(it->second) << endl;
        } // if
    };

    lookup("Charles");
    lookup("Lily");
}


int main()
{
    // if (extract_part("ABCDEFG").front() == 'C')
        // cout << "Found!" << endl;

    // substr_test();
    map_key_test();

    return 0;
}


#if 0

Construction and copying:

    basic_string_ref ();    // Constructs an empty string_ref
    basic_string_ref(const charT* str); // Constructs from a NULL-terminated string
    basic_string_ref(const charT* str, size_type len); // Constructs from a pointer, length pair

    // Constructs from a std::string
    template<typename Allocator>
    basic_string_ref(const std::basic_string<charT, traits, Allocator>& str); 

    basic_string_ref (const basic_string_ref &rhs);
    basic_string_ref& operator=(const basic_string_ref &rhs);

string_ref does not define a move constructor nor a move-assignment operator 
because copying a string_ref is just a cheap as moving one.

Basic container-like functions:

    size_type size()     const ;
    size_type length()   const ;
    size_type max_size() const ;
    bool empty()         const ;

    // All iterators are const_iterators
    const_iterator  begin() const ;
    const_iterator cbegin() const ;
    const_iterator    end() const ;
    const_iterator   cend() const ;
    const_reverse_iterator         rbegin() const ;
    const_reverse_iterator        crbegin() const ;
    const_reverse_iterator           rend() const ;
    const_reverse_iterator          crend() const ;

Access to the individual elements (all of which are const):

    const charT& operator[](size_type pos) const ;
    const charT& at(size_t pos) const ;
    const charT& front() const ;
    const charT& back()  const ;
    const charT* data()  const ;

Modifying the string_ref (but not the underlying data):

    void clear();
    void remove_prefix(size_type n);
    void remove_suffix(size_type n);

Searching:

    size_type find(basic_string_ref s) const ;
    size_type find(charT c) const ;
    size_type rfind(basic_string_ref s) const ;
    size_type rfind(charT c) const ;
    size_type find_first_of(charT c) const ;
    size_type find_last_of (charT c) const ;

    size_type find_first_of(basic_string_ref s) const ;
    size_type find_last_of(basic_string_ref s) const ;
    size_type find_first_not_of(basic_string_ref s) const ;
    size_type find_first_not_of(charT c) const ;
    size_type find_last_not_of(basic_string_ref s) const ;
    size_type find_last_not_of(charT c) const ;


String-like operations:

    basic_string_ref substr(size_type pos, size_type n=npos) const ; // Creates a new string_ref
    bool starts_with(charT c) const ;
    bool starts_with(basic_string_ref x) const ;
    bool ends_with(charT c) const ;
    bool ends_with(basic_string_ref x) const ;

#endif
