#include <iostream>

using namespace std;

template<class T>
class singleton {
public:
    struct creater {
        creater() 
        { singleton<T>::getInstance(); }

        inline void do_nothing() const {}    
    };

    static T* getInstance()
    {
        static T obj;
        cout << "getInstance" << endl;
        m_creater.do_nothing();
        return &obj;
    }

public:
    static creater m_creater;
private:
    singleton(){}
};

template<typename T> 
typename singleton<T>::creater singleton<T>::m_creater;


int main()
{
    cout << "main start..." << endl;
    singleton<int>::getInstance();

    return 0;
}
