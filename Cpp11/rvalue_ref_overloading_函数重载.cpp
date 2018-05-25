/*
 * reference collapsing rules
 * https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/reference_collapsing.htm#reference_collapsing__suplink1
 */
#include <iostream>
#include <string>
#include <type_traits>

using namespace std;

template<typename T>
void print_t(T&& i) // ?? && 最广的兼容性 universal reference
{
    ++i;
    cout << "template " << i << endl; 

    if (std::is_lvalue_reference<T>::value) {
        cout << "lvalue ref" << endl;
    } else if (std::is_rvalue_reference<T>::value) {
        cout << "rvalue ref" << endl;
    } else {
        cout << "not a ref" << endl;
    } // if
}

void print_int( int &i ) 
{ 
    cout << "lvalue reference " << i << endl; 
}

void print_int( const int &i ) 
{ cout << "const reference " << i << endl; }

void print_int( int &&i )   // ?? 6 will be 12
{ 
    i *= 2;
    cout << "rvalue reference " << i << endl; 
}

void clear_str(std::string&& s)
{ s.clear(); }

/* 
void print_int( int i )
{ cout << "value " << i << endl; }	//!! cannot overload, overload only based on reference
 */

int main()
{
	int i = 5;
	
    print_int(i);	// lvalue ref, const& & 优先选择& lvalue ref    // 选择限制最不严格的
                    // 若只有int&& 版函数，则会编译出错 必须使用print_int(std::move(i));
    cin.get();
	print_int(6);	// rvalue ref, const& && 优先选择&&

    cin.get();
    print_int(std::move(i));
    cout << "in main i = " << i << endl; // i will be set to 10

    cin.get();
    print_t(7);
    // print_t(i);     // lvalue ref
    print_t(std::move(i)); // not a ref ??
    cout << "in main i = " << i << endl; // i will be set to 10

    {
        cin.get();
        string s("hello");
        cout << "s = " << s << endl;
        clear_str(std::move(s));
        cout << "s = " << s << endl;
    }
	
	return 0;
}



