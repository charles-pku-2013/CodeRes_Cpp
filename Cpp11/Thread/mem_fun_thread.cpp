#include <cstdio>
#include <iostream>
#include <vector>
#include <thread>

class Foo final {
 public:
    void routine(int i, int j) {
        // std::cout << i * j << std::endl;
        printf("%d\n", i * j);
    }

    void run() {
        for (int i = 0; i < 10; ++i) {
            // workers_.emplace_back(std::move(std::thread(&Foo::routine, this, i, i + 1)));
            workers_.emplace_back(&Foo::routine, this, i, i + 1);
        }
        for (auto& thr : workers_) {
            if (thr.joinable()) { thr.join(); }
        }
    }

    std::vector<std::thread> workers_;
};


int main() {
    std::vector<std::thread> workers;

    Foo foo;
    foo.run();

    return 0;
}

