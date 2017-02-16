#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))

using namespace std;


// #if 0
extern char **environ;
int main(int argc, char **argv)
{
    // if (setenv("key", "value", 1) < 0)
        // cerr << strerror(errno) << endl;

    // unsetenv("key");

    // char *pEnv = getenv("key");
    // if (pEnv) cout << pEnv << endl;
    // else cout << "not found!" << endl;

    SLEEP_SECONDS(3);
    for (char **ep = environ; *ep != NULL; ep++)
        cout << *ep << endl;

    return 0;
}
// #endif


#if 0
int main()
{
    setenv("TestKey", "TestValue", 1);  // NOTE!!! 子进程继承了父进程的环境变量
    system("nohup /tmp/test > /tmp/out.txt &");
    unsetenv("TestKey");

    return 0;
}
#endif

