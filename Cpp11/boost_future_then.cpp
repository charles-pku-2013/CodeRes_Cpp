#include <iostream>
#include <string>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <boost/thread/future.hpp>

using namespace boost;

//!! then 的参数是 func_obj，其参数是前一个future的返回结果
int main() 
{
	future<int> f1 = async([]() { return 123; });

	future<std::string> f2 = f1.then([](future<int> f) {
		std::cout << f.get() << std::endl; // here .get() won't block
		return std::string("sgfsdfs");
	});

	return 0;
}