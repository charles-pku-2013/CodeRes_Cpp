#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <numeric>
#include <random>
#include <functional>


// NOTE!!! 用指针实现的
template <class T>
class my_reference_wrapper {
public:
  // types
  typedef T type;
 
  // construct/copy/destroy
  reference_wrapper(T& ref) noexcept : _ptr(std::addressof(ref)) {}
  reference_wrapper(T&&) = delete;
  reference_wrapper(const reference_wrapper&) noexcept = default;
 
  // assignment
  reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;
 
  // access
  operator T& () const noexcept { return *_ptr; }
  T& get() const noexcept { return *_ptr; }
 
  template< class... ArgTypes >
  typename std::result_of<T&(ArgTypes&&...)>::type
    operator() ( ArgTypes&&... args ) const {
    return std::invoke(get(), std::forward<ArgTypes>(args)...);
  }
 
private:
  T* _ptr;
};

struct Employee {
    std::string     name;
    uint32_t        age;
    double          salary;

    friend std::ostream& operator << (std::ostream &os, const Employee &emp)
    {
        os << emp.name << "\t" << emp.age << "\t" << emp.salary;
        return os;
    }
};


int main()
{
    using namespace std;

    vector<Employee> arrEmp;
    arrEmp.push_back(Employee{"Charles", 30, 10000});
    arrEmp.push_back(Employee{"Lee", 25, 15000});
    arrEmp.push_back(Employee{"Lucy", 28, 25000});

    // NOTE!!! 相当于是指针数组, 使得引用也可以放在容器里
    vector<reference_wrapper<Employee>> arrRefEmp;
    arrRefEmp.assign(arrEmp.begin(), arrEmp.end());

    std::sort(arrRefEmp.begin(), arrRefEmp.end(), 
        [](const Employee &lhs, const Employee &rhs)->bool{
            return lhs.age < rhs.age; });

    std::copy(arrEmp.begin(), arrEmp.end(), ostream_iterator<Employee>(cout, "\n"));
    cout << endl;
    std::copy(arrRefEmp.begin(), arrRefEmp.end(), ostream_iterator<Employee>(cout, "\n"));
    cout << endl;

    return 0;
}


#if 0
int main()
{
    std::list<int> l(10);
    std::iota(l.begin(), l.end(), -4);
 
    std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());
    // can't use shuffle on a list (requires random access), but can use it on a vector
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
 
    std::cout << "Contents of the list: ";
    for (int n : l) std::cout << n << ' '; std::cout << '\n';
 
    std::cout << "Contents of the list, as seen through a shuffled vector: ";
    for (int i : v) std::cout << i << ' '; std::cout << '\n';
 
    std::cout << "Doubling the values in the initial list...\n";
    for (int& i : l) {
        i *= 2;
    }
 
    std::cout << "Contents of the list, as seen through a shuffled vector: ";
    for (int i : v) std::cout << i << ' '; std::cout << '\n';

    return 0;
}
#endif
