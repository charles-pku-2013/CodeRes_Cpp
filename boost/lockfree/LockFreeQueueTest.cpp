/*
 * c++ -o /tmp/test LockFreeQueueTest.cpp -pthread -std=c++11 -g
 *
 * NOTE!!!
 * push: 不受指定size影响
 * bounded_push: 受指定size限制
 */
#include <iostream>
#include <map>
#include <boost/format.hpp>
#include "LockFreeQueue.hpp"

using namespace std;

int main(int argc, char **argv)
{
    LockFreeQueue<int> que(5);

    cout << que.is_lock_free() << endl;

    for (int i = 1; i <= 10; ++i) {
        bool ret = que.push(i, 1000);
        if (!ret) {
            cout << boost::format("push %d fail") % i << endl;
        } // if
    } // for

    int val = 0;
    while (que.pop(val, 1000))
        cout << val << endl;


    // int val = 0;
    // bool ret = que.pop(val, 1000);
    // if (!ret) cout << "pop fail" << endl;

    return 0;
}

