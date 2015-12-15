#include <iostream>
#include <memory>
 
using namespace std;

std::weak_ptr<int> gw;

struct Foo {
	void greet() { cout << "Hello" << endl; }
};
 
void f()
{
    if (auto spt = gw.lock()) { // Has to be copied into a shared_ptr before usage
		std::cout << *spt << "\n";
    } else {
        std::cout << "gw is expired\n";
    }
}
 
int main()
{
	// test
    {
        auto sp = std::make_shared<int>(42);
		gw = sp;
		f();
    }
 
    f();
	
	std::weak_ptr<Foo> wp;
	// test 
	{
		auto sp = std::make_shared<Foo>();
		cout << sp.use_count() << endl;
		wp = sp;
		// wp.lock()->greet();
	}
	cout << wp.use_count() << endl;
	// wp->greet(); //!! 不可以这样使用, weak_ptr 没有一般的指针操作符
	wp.lock()->greet();		//!! 不应该不判断就直接使用，很多情况下不会报错
	if( auto a = wp.lock() )
		a->greet();
	else
		cout << "Invalid pointer" << endl;
	
	return 0;
}













