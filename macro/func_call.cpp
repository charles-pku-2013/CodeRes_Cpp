#define BUILD_TENSOR(func, ...)   \
    do {  \
        std::unique_ptr<Tensor> p_tensor(new Tensor);  \
        p_tensor->func(__VA_ARGS__);  \
        element.tensor = p_tensor.get();  \
        tensors.emplace_back(std::move(p_tensor));  \
        element.success = true;  \
    } while (0)

BUILD_TENSOR(AssignSingleValue, val);
BUILD_TENSOR(AssignVector, values, true);
