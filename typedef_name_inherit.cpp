#include <vector>
#include <iostream>

/*
 * 从基类继承name，本类不能是模板。
 */

// That's perfect OK
struct IntVector : std::vector<int> {
    pointer         ptr;
};

template <typename T>
struct GeneralArray : std::vector<T> {
    // That's OK
    typedef typename std::vector<T>      BaseType;
    typedef typename BaseType::pointer   pointer;

    // without above typedefs
    // pointer         ptr; //!! error, unknown type
};


int main()
{

    
    return 0;
}

