#include <cstdio>


//!! Outter Inner 是两个完全独立的类，无任何依赖关系。
//!! Outter Inner 只确定了NameSpace包含关系
class Outter {
public:
    Outter()    //!! will never called when an Inner obj constructed
    { printf("Outter constructor\n"); }
    
    int outNum;
    
    class Inner {
    public:
        Inner()     //!! will never called when an Outter obj constructed
        { printf("Inner constructor\n"); }
        int inNum;
    };
};


int main()
{
//    Outter::Inner obj;
//    obj.inNum = 5;
//    obj::outNum = 7;
    
    Outter obj;
    printf("%lu\n", sizeof(obj));       //!! only 4, one int
    Outter::Inner inObj;
    
    return 0;
}















