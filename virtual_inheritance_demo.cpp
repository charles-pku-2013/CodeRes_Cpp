#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>


using namespace std;


class Animal {
public:
    // Animal()
    // { printf("Animal constructor at %lx\n", (long)this); }
    virtual void eat(){}
    // virtual void eat() = 0;
    int x;
};

class Mammal : public virtual Animal {
public:
    // Mammal()
    // { printf("Mammal constructor at %lx\n", (long)this); }
    virtual void breathe(){}
    // void eat() {}
};

class WingedAnimal : public virtual Animal {
public:
    // WingedAnimal()
    // { printf("WingedAnimal constructor at %lx\n", (long)this); }
    virtual void flap(){}
    // void eat() {}
};

// A bat is a winged mammal
class Bat : public Mammal, public WingedAnimal {
};

int main()
{
    // test without virtual
    /*
     * {
     *     Bat bat;
     *     // bat.eat();
     *     // error: request for member ‘eat’ is ambiguous
     *     // Animal &a = bat; 
     *     // Animal *a = &bat;
     *     // error: ‘Animal’ is an ambiguous base of ‘Bat’
     *     // Animal &a = static_cast<Mammal&>(bat);      // correct
     *     // a.eat();
     *     // bat.x = 5;
     *     // error: request for member ‘x’ is ambiguous
     *     bat.Mammal::x = 5;
     *     bat.WingedAnimal::x = 7;        // correct
     *     printf("Addr of Mammal::x is %lx, of WingedAnimal::x is %lx\n", (long)(&(bat.Mammal::x)), (long)(&(bat.WingedAnimal::x)));
     *     printf("Value of Mammal::x is %d, of WingedAnimal::x is %d\n", bat.Mammal::x, bat.WingedAnimal::x);
     *     // Addr of Mammal::x is 7ffcd1939888, of WingedAnimal::x is 7ffcd1939898
     *     // Value of Mammal::x is 5, of WingedAnimal::x is 7
     * }
     */

    // test with virtual
    {
        Bat bat;
        // bat.eat();      // OK
        bat.x = 5;
        bat.Mammal::x = 7;
        printf("Addr of Mammal::x is %lx, of WingedAnimal::x is %lx\n", (long)(&(bat.Mammal::x)), (long)(&(bat.WingedAnimal::x)));
        printf("Value of Mammal::x is %d, of WingedAnimal::x is %d\n", bat.Mammal::x, bat.WingedAnimal::x);
        // Addr of Mammal::x is 7ffc98664628, of WingedAnimal::x is 7ffc98664628
        // Value of Mammal::x is 7, of WingedAnimal::x is 7
    }

    // virtual inheritance test
    /*
     * {
     *     Mammal m;
     *     WingedAnimal w;
     *     m.x = 5;
     *     w.x = 7;
     *     printf("%d\n", m.x);
     *     //!! result is 5, m and w are two different objects
     * }
     */

    // pure virtual function in Animal
    /*
     * {
     *     // if eat() defined pure virtual and implemented in both Mammal and WingedAnimal
     *     // but it's ok if only Mammal or WingedAnimal provide the implement.
     *     Bat bat;
     *     // error: no unique final overrider for ‘virtual void Animal::eat()’ in ‘Bat’
     * }
     */


    return 0;
}
