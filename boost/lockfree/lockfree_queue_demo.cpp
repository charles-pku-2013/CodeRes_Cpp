/*
 * c++ -o /tmp/test lockfree_queue_demo.cpp -lboost_thread -lboost_system -std=c++11 -g
 */
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <boost/atomic.hpp>

/*
 * 只能存放基本类型数据
 * error: static assertion failed: (boost::has_trivial_destructor<T>::value)
 *      BOOST_STATIC_ASSERT((boost::has_trivial_destructor<T>::value));
 */
boost::atomic_int producer_count(0);
boost::atomic_int consumer_count(0);

// 初始size参数不是必须的
boost::lockfree::queue<int> queue(128);

const int iterations = 10000000;
const int producer_thread_count = 4;
const int consumer_thread_count = 4;

void producer(void)
{
    for (int i = 0; i != iterations; ++i) {
        int value = ++producer_count;
        while (!queue.push(value))
            boost::this_thread::yield();
    } // for
}

boost::atomic<bool> done (false);
void consumer(void)
{
    int value;
    while (!done) {
        while (queue.pop(value))
            ++consumer_count;
        boost::this_thread::yield();
    } // while

    while (queue.pop(value))
        ++consumer_count;
}

int main(int argc, char* argv[])
{
    using namespace std;

    cout << "boost::lockfree::queue is ";
    if (!queue.is_lock_free())
        cout << "not ";
    cout << "lockfree" << endl;

    boost::thread_group producer_threads, consumer_threads;

    for (int i = 0; i != producer_thread_count; ++i)
        producer_threads.create_thread(producer);

    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(consumer);

    producer_threads.join_all();
    done = true;

    consumer_threads.join_all();

    cout << "produced " << producer_count << " objects." << endl;
    cout << "consumed " << consumer_count << " objects." << endl;

    return 0;
}
