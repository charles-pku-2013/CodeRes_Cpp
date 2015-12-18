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
	std::unique_ptr<std::thread> pThread(new std::thread(thread_routine));
	std::this_thread::sleep_for(std::chrono::seconds(3));
	active = false;				// 虽然线程函数执行完了，但没有join，没有detach，还是不能直接delete
	//pThread->detach();		// detach之后在delete是没有问题的。
	if (pThread->joinable())	// join 过的线程可以delete
		pThread->join();
	pThread.reset();

    return 0;
}
