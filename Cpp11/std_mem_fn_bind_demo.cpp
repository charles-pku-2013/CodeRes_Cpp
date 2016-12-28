#include <functional>
#include <iostream>
 
struct Foo {
    void display_greeting() {
        std::cout << "Hello, world.\n";
    }
    void display_number(int i) {
        std::cout << "number: " << i << '\n';
    }
    int data = 7;
};
 
int main() {
    Foo f;
    Foo *pf = &f;
 
    auto greet = std::mem_fn(&Foo::display_greeting);
    greet(f);
    greet(pf);
 
    auto print_num = std::mem_fn(&Foo::display_number);
    print_num(f, 42);
    print_num(pf, 42);

    // NOTE!!! how bind works on member function
    auto fn = std::bind(print_num, f, 24);
    fn();
 
    auto access_data = std::mem_fn(&Foo::data);
    std::cout << "data: " << access_data(f) << '\n';

    return 0;
}
