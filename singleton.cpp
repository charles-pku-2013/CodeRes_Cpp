#include <iostream>

using namespace std;

struct Foo {
    Foo() { cout << "Foo construct" << endl; }
    ~Foo() { cout << "Foo destruct" << endl; }
    void greet() const { cout << "Foo greet" << endl; }
};

template<class T>
class singleton {
public:
    struct creater {
        creater() 
        { 
            cout << "singleton creater construct" << endl;
            singleton<T>::getInstance(); 
        }

        inline void do_nothing() const {}    
    };

    static T* getInstance()
    {
        static T obj;
        cout << "getInstance" << endl;
        // m_creater.do_nothing();
        return &obj;
    }

public:
    static creater m_creater;
private:
    singleton() {}
};

template<typename T> 
typename singleton<T>::creater singleton<T>::m_creater;


int main()
{
    cout << "main start..." << endl;

    // singleton<Foo>::m_creater; // 当引用了其静态成员时，会在main函数之前初始化

    do {
        singleton<Foo>::getInstance(); // 要确保静态成员变量被引用，进而使编译器决定在main之前初始化
    } while (0);

    do {
        singleton<Foo>::getInstance()->greet();
    } while (0);

    return 0;
}
