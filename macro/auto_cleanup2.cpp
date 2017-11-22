#include <iostream>

using namespace std;

// 可以手动释放
#define AUTO_CLEANUP(name, codeBlock)                                              \
    struct __type_##name {                                                         \
        __type_##name() : bReleased_(false) {}                                     \
        ~__type_##name() { release(); }                                            \
        void release() { if (!bReleased_) { _doRelease(); bReleased_ = true; } }   \
        void _doRelease() codeBlock                                                \
        bool bReleased_;                                                           \
    } __##name;


void test()
{
    cout << "test begin" << endl;

    AUTO_CLEANUP(cleanup, {
        cout << "auto cleanup1" << endl;
        int sum = 0;
        for (int i = 1; i <= 100; ++i)
            sum += i;
        cout << "auto cleanup2 sum = " << sum << endl;
    });

    __cleanup.release();

    cout << "test end" << endl;
}

int main(int argc, char **argv)
try {
    test();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}


