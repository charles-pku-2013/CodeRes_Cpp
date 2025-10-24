template <typename T>
class MyTemplateClass {
public:
    MyTemplateClass(T value) : data_(value) {}
    T get_data() const { return data_; }
    void set_data(T value) { data_ = value; }
private:
    T data_;
};

#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

template <typename T>
void declare_my_template_class(py::module &m, const std::string &name_suffix) {
    using ClassType = MyTemplateClass<T>;
    std::string py_class_name = std::string("MyTemplateClass") + name_suffix;
    py::class_<ClassType>(m, py_class_name.c_str())
        .def(py::init<T>())
        .def("get_data", &ClassType::get_data)
        .def("set_data", &ClassType::set_data);
}

PYBIND11_MODULE(my_module, m) {
    declare_my_template_class<int>(m, "Int");
    declare_my_template_class<double>(m, "Double");
    // Add more instantiations as needed
}
