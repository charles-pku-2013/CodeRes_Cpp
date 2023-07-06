#include <stdio.h>

extern char **environ;

int main() {
    char **var = environ;
    for (; *var; ++var) {
        std::cerr << *var << std::endl;
    }

    return 0;
}
