#include <memory>
#include <iostream>
#include <functional>
#include <cstdio>
#include <boost/format.hpp>

#define ON_FINISH(name, deleter) \
    std::unique_ptr<void, std::function<void(void*)> > \
        name((void*)-1, [&](void*) deleter )

#define ON_FINISH_CLASS(name, deleter) \
    std::unique_ptr<void, std::function<void(void*)> > \
        name((void*)-1, [&, this](void*) deleter )

#define ON_FINISH_TYPE(type, name, ptr, deleter) \
    std::unique_ptr<type, std::function<void(type*)> > \
        name((ptr), [&](type* pArg) deleter )

using namespace std;

void test1()
{
    auto deleter = [&](void*) {
        cout << "Deleter called" << endl;
    };

    // !!!NOTE unique_ptr 析构时会先判断是否是空指针，只有不空时才调用deleter
    // shared_ptr暂时没发现这个问题，为了保险起见，这种应用场景不应该使用 (void*)0
    std::unique_ptr<void, decltype(deleter)> pTest((void*)-1, deleter);

    // std::shared_ptr<void> pTest2((void*)NULL, [&](void*){
        // cout << "Deleter of shared_ptr!" << endl;
    // });
}

void test2()
{
    // OK
    // std::unique_ptr<void, void (*)(void*)> pTest((void*)-1, [&](void*){
        // cout << "unique_ptr deleter called." << endl;
    // });

    // OK
    std::unique_ptr<void, std::function<void(void*)>> pTest((void*)-1, [&](void*){
        cout << "unique_ptr deleter called." << endl;
    });

    // WRONG!!!
    // std::unique_ptr<void, void(void*)> pTest((void*)-1, [&](void*){
        // cout << "unique_ptr deleter called." << endl;
    // });
}

void test3()
{
    ON_FINISH(_pCleanup, {
        cout << "ON_FINISH called" << endl;
        cout << "ON_FINISH called again" << endl;
    });
}

void test4()
{
    ON_FINISH_TYPE(FILE, fp, fopen("/tmp/test.txt", "w"), {
        cout << "Closing file..." << endl;
        fclose(pArg);
    });

    fprintf(fp.get(), "hello\n");
}

void test5()
{
#if 0
    // NOTE!!! unique_ptr的reset函数不支持指定定制的deleter实例
    std::unique_ptr<char, std::function<void(char*)>> ptr;
    char *data = new char[100];
    ptr.reset(data, [](char* p){
        if (p) {
            cout << "custom deleter called!" << endl;
            delete [] p;
        }
    });
#endif

#if 0
    // NOTE!!! 使用reset正确的方式
    struct _Deleter {
        void operator()(char *p) {
            if (p) {
                cout << "custom deleter called! Addr: " << boost::format("%lx") % (void*)p << endl;
                delete [] p;
            }
        }
    };
    std::unique_ptr<char, _Deleter> ptr;  // call the default construct of _Deleter
    char *data = new char[100];
    cout << "Addr of data: " << boost::format("%lx") % (void*)data << endl;
    ptr.reset(data);
#endif

    // NOTE!!! This also OK!
    std::unique_ptr<char, std::function<void(char*)>> ptr(nullptr, [](char* p){
        if (p) {
            cout << "custom deleter called! Addr: " << boost::format("%lx") % (void*)p << endl;
            delete [] p;
        }
    });
    char *data = new char[100];
    cout << "Addr of data: " << boost::format("%lx") % (void*)data << endl;
    ptr.reset(data);
}


int main()
{
    // test1();
    // test2();
    // test3();
    // test4();
    test5();

    return 0;
}
