#include <thread>
#include <future>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
 
//!! promise 不要让我等得太久, 对应future
int main()
{
    std::istringstream iss_numbers{"3 4 1 42 23 -23 93 2 -289 93"};
    std::istringstream iss_letters{" a 23 b,e a2 k k?a;si,ksa c"};
 
    std::vector<int> numbers;		//!! 对应的future也是void
    std::vector<char> letters;
    std::promise<void> numbers_promise, letters_promise;
 
    auto numbers_ready = numbers_promise.get_future();
    auto letter_ready = letters_promise.get_future();
 
    std::thread value_reader([&]
    {	// run in seperate thread
        // I/O operations.
        std::copy(std::istream_iterator<int>{iss_numbers},
                  std::istream_iterator<int>{},
                  std::back_inserter(numbers));
 
        //Notify for numbers.
        numbers_promise.set_value();	//!! 此例中，promise-future 像条件变量，等待条件设置
 
        std::copy_if(std::istreambuf_iterator<char>{iss_letters},
                     std::istreambuf_iterator<char>{},
                     std::back_inserter(letters),
                     ::isalpha);
 
        //Notify for letters.
        letters_promise.set_value();
    });
 
	// in main thread
    numbers_ready.wait();		// future
 
    std::sort(numbers.begin(), numbers.end());
 
    if (letter_ready.wait_for(std::chrono::seconds(1)) ==
            std::future_status::timeout)
    {
        //output the numbers while letters are being obtained.
        for (int num : numbers) std::cout << num << ' ';
        numbers.clear(); //Numbers were already printed.
    }
 
    letter_ready.wait();
    std::sort(letters.begin(), letters.end());
 
    //If numbers were already printed, it does nothing.
    for (int num : numbers) std::cout << num << ' ';
    std::cout << '\n';
 
    for (char let : letters) std::cout << let << ' ';
    std::cout << '\n';
 
    value_reader.join();
}




















