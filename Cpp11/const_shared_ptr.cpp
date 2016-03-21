#include <iostream>
#include <memory>

using namespace std;


void test1()
{
    std::shared_ptr<int> p = std::make_shared<int>(5);
    std::shared_ptr<const int> cp;
    cp = p;
    // *cp = 10;
    
    // std::weak_ptr<int> wp = cp;  // error, cannot assign const to non-const
    std::weak_ptr<const int> cwp = p; // OK, non-const to const
    auto ap = cwp.lock();
    // *ap = 10;
    
    cout << p.use_count() << endl;
}


int main()
{
    test1();

    /*
     * const std::shared_ptr<int> spInt = std::make_shared<int>(5);
     * // spInt.reset(); // error, this const shared_pointer is identical to T *const pointer.
     * *spInt = 10;        // OK
     */

    // std::shared_ptr<const int> spInt = std::make_shared<int>(5);
    // error, it's identical to const int *p or int const *p
    // a pointer to const type
    // *spInt = 10;
    // spInt.reset();   // OK

    // cout << *spInt << endl;

    return 0;
}













