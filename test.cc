#include <iostream>

struct InferArgs {
    std::size_t n_imgs_ = 0;
    bool        is_np_ = false;
    float       mean_ = 0.0f;
    float       std_ = 0.0f;
    bool        skip_resize_ = false;
    std::size_t result_offset_ = 0;
    char        shm_name_[64]{0};
};

int main() {
    using namespace std;

    InferArgs arg{0, true, 1.0f, 2.0f, "hello"};

    return 0;
}
