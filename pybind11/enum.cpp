/*
https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/05.%E9%9D%A2%E5%AF%B9%E5%AF%B9%E8%B1%A1%E7%BC%96%E7%A8%8B.md

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) object_oriented.cpp -o example$(python3-config --extension-suffix)
MacOS
c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup $(python3 -m pybind11 --includes) enum.cpp -o example$(python3-config --extension-suffix)
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age = 0;
    };

    Pet(const std::string &name, Kind type) : name(name), type(type) { }

    std::string name;
    Kind type;
    Attributes attr;
};

PYBIND11_MODULE(example, m) {
    py::class_<Pet> pet(m, "Pet");

    pet.def(py::init<const std::string&, Pet::Kind>())
        .def_readwrite("name", &Pet::name)
        .def_readwrite("type", &Pet::type)
        .def_readwrite("attr", &Pet::attr);

    py::enum_<Pet::Kind>(pet, "Kind")
        .value("Dog", Pet::Kind::Dog)
        .value("Cat", Pet::Kind::Cat)
        .export_values();

    py::class_<Pet::Attributes> attributes(pet, "Attributes");
    attributes.def(py::init<>())
              .def_readwrite("age", &Pet::Attributes::age);
}

#if 0
>>> p = example.Pet("Lucy", example.Pet.Cat)
>>> p.type
<Kind.Cat: 1>
>>> example.Pet.Kind.__members__
{'Dog': <Kind.Dog: 0>, 'Cat': <Kind.Cat: 1>}
#endif

