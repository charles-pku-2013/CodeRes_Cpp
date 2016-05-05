#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

class times_two_visitor
        : public boost::static_visitor<> { // 空模板 apply_visitor 不需要返回值
public:
    void operator()(int& i) const
    { i *= 2; }

    void operator()(std::string& str) const
    { str += str; }
};

class times_two_generic
        : public boost::static_visitor<> {
public:
    template <typename T>
    void operator()( T& operand ) const
    { operand += operand; }
};

static
void test1()
{
    boost::variant< int, std::string > v;
    v = "hello";
    std::cout << v << std::endl;
    // test get
    std::string& str = boost::get<std::string>(v);
    str += " world! ";
    std::cout << v << std::endl;

    try {
        int& n = boost::get<int>(v);
    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try
    // boost::bad_get: failed value get using boost::get

    // test apply_visitor
    // v = 5;
    // boost::apply_visitor( times_two_visitor(), v );
    // std::cout << v << std::endl;
    boost::apply_visitor( times_two_generic(), v );
    std::cout << v << std::endl;

    exit(0);
}

static
void test2()
{
    typedef boost::variant<int, std::string> va_type;

    std::vector< va_type > vec;
    vec.push_back( 21 );
    vec.push_back( "hello " );

    // wrong
    // std::copy( vec.begin(), vec.end(), ostream_iterator<va_type>(cout, '\n') );
    
    for (const auto &v : vec)
        cout << v << endl;

    times_two_generic visitor;
    std::for_each(
            vec.begin(), vec.end(), 
            boost::apply_visitor(visitor));

    for (const auto &v : vec)
        cout << v << endl;

    exit(0);
}


int main()
{
    test1();
    // test2();

    return 0;
}
