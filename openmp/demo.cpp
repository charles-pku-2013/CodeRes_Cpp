/*
 * compile: c++ -o /tmp/test demo.cpp -std=c++11 -fopenmp -g
 * 主流C/C++编译器，如gcc与visual C++，都内在支持OpenMP。一般都必须在程序中 #include <omp.h>
 * gcc编译时需使用编译选项-fopenmp。但是，如果编译为目标文件与链接生成可执行文件是分开为两步操作，
 * 那么链接时需要给出附加库gomp，否则会在链接时报错“undefined reference to `omp_get_thread_num'"。
 */
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <omp.h>

using namespace std;


template <typename Container>
void copy_pointer(  Container &c, std::vector<typename Container::value_type*> &ptrArr )
{
    ptrArr.clear();
    ptrArr.reserve(c.size());
    for (auto it = c.begin(); it != c.end(); ++it)
        ptrArr.push_back( &(*it) );
}


int main()
{
    typedef std::map<std::string, int> StrUIntMap;

    auto process = [](StrUIntMap::value_type &v) {
        // 若不用{} 则只对随后的一句起作用
#pragma omp critical
        cout << "In thread " << std::this_thread::get_id() << " " << v.first << " = " << v.second << endl;
        v.second *= 2;
        std::this_thread::sleep_for(std::chrono::seconds(1));
#pragma omp critical
        cout << "Thread " << std::this_thread::get_id() << " Done!" << endl;
    };

    auto processArr = [](int &v) {
        v *= 2;
#pragma omp critical
        cout << "In thread " << std::this_thread::get_id() << " value = " << v << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
#pragma omp critical
        cout << "Thread " << std::this_thread::get_id() << " Done!" << endl;
    };

    StrUIntMap m;
    m["hello"] = 10;
    m["world"] = 20;
    m["pku"] = 100;
    m["sun"] = 50;
    m["Charles"] = 90;
    m["laji"] = -100;
    m["tank"] = 25;
    m["openmp"] = 60;
    m["Intel"] = 85;
    m["Baidu"] = -1000;

    // wrong
// #pragma omp parallel for
    // for (auto it = m.begin(); it != m.end(); ++it) {
        // process(*it);
    // } // for

    // auto it = m.begin();
// #pragma omp parallel for
    // for (size_t i = 0; i < m.size(); ++i) {
        // process(*it++);
    // } // for
/*
 * In thread 140408659560384 hello = 10
 * In thread 140408632211200 hello = 10
 * In thread 140408640603904 hello = 10
 */


    /*
     * How to loop over with iterator:
     * Bellow will execute the loop in one thread, but delegate the processing of elements to others.
     * OpenMP 3.0 above is required
     */
// #pragma omp parallel
// #pragma omp single
    // {
    // for (auto it = m.begin(); it != m.end(); ++it)
// #pragma omp task firstprivate(it)
            // process(*it);
// #pragma omp taskwait
    // }

    /*
     * Without OpenMP 3.0 the easiest way would be writing all pointers to elements 
     * in the list (or iterators in a vector and iterating over that one. 
     */
    vector< StrUIntMap::value_type* > pArr;
    copy_pointer(m, pArr);
#pragma omp parallel for
    for (size_t i = 0; i < pArr.size(); ++i)
        process( *pArr[i] );

    // vector<int> arr = {1,2,3,4,5,6};
// #pragma omp parallel for
    // for (size_t i = 0; i < arr.size(); ++i)
        // processArr(arr[i]);

    // 不可以用 i != arr.size() 判断循环结束条件 error: invalid controlling predicate
/*
 * In thread 140565560051456 value = 6
 * In thread 140565560051456 value = 8
 * In thread 140565543266048 value = 12
 * In thread 140565551658752 value = 10
 * In thread 140565579007936 value = 2
 * In thread 140565579007936 value = 4
 */

    return 0;
}
