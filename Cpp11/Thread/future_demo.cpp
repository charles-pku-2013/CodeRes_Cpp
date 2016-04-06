#include <thread>
#include <chrono>
#include <future>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


using namespace std;
 

// compute the sum of [beg:end) starting with the initial value init
int accum(int* beg, int* end, int init)
{   
    thread::id this_id = this_thread::get_id();
    cout << "In thread " << this_id << endl;
    
    this_thread::sleep_for( chrono::seconds(1) );
    
    return accumulate( beg, end, init );
}

int comp2(vector<int>& v)
{
    using Task_type = int(int*,int*,int); // type of task
    packaged_task<Task_type> pt0 {accum}; // package the task (i.e., accum)
    packaged_task<Task_type> pt1 {accum};
    future<int> f0 {pt0.get_future()}; // get hold of pt0’s future
    future<int> f1 {pt1.get_future()}; // get hold of pt1’s future
    int *first = &v[0];
    thread t1 { std::move(pt0), first,first+v.size()/2, 0 }; // star t a thread for pt0
    thread t2 { std::move(pt1), first+v.size()/2, first+v.size(), 0 }; // star t a thread for pt1
    t1.detach();            //!! 要么join要么detach，否则core dump
    t2.detach();
    // t1.join();
    // t2.join();
    
    return f0.get() + f1.get(); // get the results
    // return 0;
}
 
 
int main()
{
    vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    
    try {
        int sum = comp2(v1);
        cout << "In main(): " << sum << endl;
    } catch( const exception &ex ) {
        cout << "In main() exception caught: " << ex.what() << endl;
    }
    
    return 0;
}








 
 
 
 
/*  
#include <iostream>
#include <chrono>
#include <thread>
 
int main()
{
    // using namespace std::literals;
    std::cout << "Hello waiter" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for( std::chrono::seconds(1) );
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
}
  */
 
 
 
 
 
 
 
 
 
