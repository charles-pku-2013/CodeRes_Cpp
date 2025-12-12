#include <iostream>
#include <utility>

using namespace std;

#if 0
实际输入参数类型   	 std::forward 模板参数 T 的推导/指定类型 	 std::forward 的返回类型 	 结果值的类别
左值（如变量）     	 Type& (左值引用)                        	 Type&                   	 左值
左值引用           	 Type& (左值引用)                        	 Type&                   	 左值
右值（如临时对象） 	 Type (非引用)                           	 Type&&                  	 右值
右值引用           	 Type&& (右值引用)                       	 Type&&                  	 右值
#endif

// --- 1. 辅助函数：用于区分左值和右值 ---

// 处理左值引用 (Lvalue reference)
void handle_value(std::string& lvalue) {
    std::cout << "  [处理函数] 接收到一个左值: " << lvalue << std::endl;
}

// 处理右值引用 (Rvalue reference)
void handle_value(std::string&& rvalue) {
    std::cout << "  [处理函数] 接收到一个右值: " << rvalue << std::endl;
}

// --- 2. 转发函数：使用 std::forward ---

// T 是一个万能引用（转发引用）
template <typename T>
void wrapper(T&& arg) {
    std::cout << "[包裹函数] 原始参数类型被推导为: " << typeid(T).name() << std::endl;
    std::cout << "[包裹函数] 准备转发参数..." << std::endl;

    // 使用 std::forward 进行完美转发
    handle_value(std::forward<T>(arg)); 
}

// --- 3. 测试用例 ---

int main() {
    std::cout << "--- 测试 1: 传递一个具名左值变量 ---" << std::endl;
    std::string s = "hello"; // s 是一个左值
    wrapper(s); 
    // 预期：wrapper内部调用 handle_value(std::string&)，因为 s 是左值。

    std::cout << "\n--- 测试 2: 传递一个右值（临时字符串） ---" << std::endl;
    wrapper(std::string("world")); 
    // 预期：wrapper内部调用 handle_value(std::string&&)，因为临时对象是右值。
    
    std::cout << "\n--- 测试 3: 传递一个左值的移动语义（强制转换为右值引用） ---" << std::endl;
    wrapper(std::move(s));
    // 预期：wrapper内部调用 handle_value(std::string&&)，尽管 s 原本是左值，但被 move 强制转换为右值引用类型。

    return 0;
}

/*
void func2( int &&j )
{
	cout << j << endl;
}

void func1( int &&i )
{
	//!! named rvalue reference is lvalue
	func2(i);	// error: cannot bind ‘int’ lvalue to ‘int&&’
}
 */


/*
// std::forward implement
template<typename T>                // For lvalues (T is T&),
T&& std::forward(T&& param)         // take/return lvalue refs.
{                                   // For rvalues (T is T),
    return static_cast<T&&>(param); // take/return rvalue refs.
}
 */

void f( int v1, int &v2 )
{
	cout << "In f(): " << v1 << " " << ++v2 << endl;
}

void g( int &&i, int &j )
{
	cout << "In g(): " << i << " " << j << endl;
}

template < typename F, typename T1, typename T2 >
void flip( F f, T1 t1, T2 t2 )
{
	f( t2, t1 );
}


template < typename F, typename T1, typename T2 >
void flip2( F fp, T1 &&t1, T2 &&t2 )
{
	//!! named rvalue ref is lvalue
	fp( t2, t1 );	//!! here t2 is lvalue.
}


template < typename F, typename T1, typename T2 >
void flip3( F fp, T1 &&t1, T2 &&t2 )
{
	fp( std::forward<T2>(t2), std::forward<T1>(t1) ); //!! std::forward<> template arg must be provided
}


int main()
{
/*
	int i = 5;
	func1(i);
 */

	int i = 5;
	f(42, i);
	cout << "In main(): i = " << i << endl;

	int j = 10;
	//!! Even through f() v2 is ref, j passed to flip by value, change in f cannot reflect back.
	// flip( f, j, 42 );
	// flip2( g, j, 42 );	// error: cannot bind ‘int’ lvalue to ‘int&&’
	flip3( g, j, 42 );
	cout << "In main(): j = " << j << endl;

	return 0;
}












