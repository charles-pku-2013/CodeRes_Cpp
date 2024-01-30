#include <iostream>
using namespace std;

class A {
public:
    static A& Instance() {
        static A inst;
        return inst;
    }

    void Greet()
    { cout << "hello" << endl; }
private:
    ~A(){
        cout << "destructor is called" << endl;
    }
};

int main(){
    A::Instance().Greet();
   return 0;
}
