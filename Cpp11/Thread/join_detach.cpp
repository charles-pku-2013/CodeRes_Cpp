#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

struct Foo {
	Foo()
	{
		cout << "Foo constructor." << endl;
	}

	~Foo()
	{
		cout << "Foo destructor." << endl;
	}
};


bool active;

void thread_routine()
{
	std::unique_ptr<Foo> ptr(new Foo);

    static uint32_t count = 0;
    while( active ) {
        cout << "thread running " << ++count << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    cout << "thread routine end." << endl;
}


int main()
{
    active = true;
    std::thread thr( thread_routine );
	thr.detach();
    std::this_thread::sleep_for(std::chrono::seconds(5));
	active = false;
    //active = false;

    /*
     * thr.join();
	 * cout << "join return" << endl;
     * 在windows下join一个已经完成join的thread会抛出异常。应该先用joinable()判断
	 * detach之后 joinable为false
     * // thr.join();
     */

	std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
