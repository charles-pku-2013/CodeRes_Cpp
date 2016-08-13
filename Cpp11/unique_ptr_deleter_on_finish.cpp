#include <memory>
#include <iostream>
#include <functional>
#include <cstdio>

#define ON_FINISH(name, deleter) \
    std::unique_ptr<void, std::function<void(void*)> > \
        name((void*)-1, [&](void*) deleter )

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

int main()
{
    // test1();
    // test2();
    // test3();
    test4();

    return 0;
}

