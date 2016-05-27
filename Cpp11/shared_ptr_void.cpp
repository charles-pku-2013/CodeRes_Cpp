// 功能：在退出作用域时执行特定函数清理工作
#include <memory>
#include <iostream>

using namespace std;

static shared_ptr<void> g_ptr;

static
void g_finish(void*)
{
    cout << "g_finish() called." << endl;
}

static
void finish(void*)
{
    cout << "finish() called." << endl;
}

int main()
{
    shared_ptr<void> p( (void*)0, finish );
    g_ptr.reset((void*)0, g_finish);

    cout << "In main()." << endl;

    return 0;
}

