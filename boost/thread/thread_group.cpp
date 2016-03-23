// compile: c++ -o /tmp/test thread_group.cpp -std=c++11 -pthread -lboost_thread -lboost_system -g

#include <boost/thread.hpp>
#include <boost/chrono/chrono.hpp>
#include <iostream>
#include <functional>

using namespace std;

int main()
{
    boost::mutex        cout_mutex;

    auto threadFunc = [&]( int index ) {
        for( int i = 0; i < 5; ++i ) {
            boost::unique_lock< boost::mutex > lock(cout_mutex);
            cout << "Thread " << index << " id is " 
                << boost::this_thread::get_id() << " working." << endl;
            lock.unlock();
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        } // for
    };

    boost::thread_group thrgroup;

    for( int i = 1; i <= 5; ++i )
        thrgroup.create_thread( std::bind(threadFunc, i) );

    thrgroup.join_all();

    return 0;
}

