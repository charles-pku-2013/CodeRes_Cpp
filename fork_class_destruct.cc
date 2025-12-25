#include <iostream>
#include <memory>
#include <unistd.h>

struct Foo {
    static Foo& instance() {
        static Foo inst;
        return inst;
    }

    void greet() {
        printf("hello\n");
    }

    ~Foo() {
        printf("Foo destruct\n");
    }
};

int main() {
    // auto ptr = std::make_unique<Foo>();  // 这种情况Foo destrcut只执行一次
    Foo::instance().greet();   // 这种情况Foo destrcut执行2次

    pid_t pid = fork();

    if (pid == 0) {
        printf("Child\n");
        _exit(0);  // exit(0) 函数并不会触发局部（自动）变量的析构函数
                   // 要想让Foo destruct只在父进程执行一次，务必使用_exit
                   // 在绝大多数涉及 fork 的场景中，_exit是最佳实践，因为它能防止子进程干扰父进程共享的资源（如文件、Socket 等）。
    } else {
        printf("Parent\n");
    }

    return 0;
}

