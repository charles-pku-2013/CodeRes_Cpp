#include <iostream>
#include <cstdio>

int main() {
    printf("date: '%s'\n", __DATE__);
    printf("time: '%s'\n", __TIME__);
    printf("Build time: '%s'\n", __TIMESTAMP__);

    return 0;
}

