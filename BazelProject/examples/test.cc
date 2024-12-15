#include <iostream>
// #include "absl/strings/str_format.h"

int main() {
    using namespace std;

    // cout << absl::StrFormat("Build time: %s", __TIMESTAMP__) << endl;
    printf("Build time: %s\n", __TIMESTAMP__);

    return 0;
}

