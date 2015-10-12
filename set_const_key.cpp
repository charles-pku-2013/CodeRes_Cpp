#include <cstdio>
#include <set>
#include <string>

using namespace std;

struct Item {
    string name;
    int count;
    
    Item( const char *_name ) : name(_name), count(1) {}
    void addCount() { ++count; }
    
    bool operator < ( const Item &rhs ) const
    { return name < rhs.name; }
};

int main()
{
    set<Item> itemSet;
    itemSet.insert( "hello" );
    
    set<Item>::iterator it = itemSet.find("hello");
    printf("%s %d\n", it->name.c_str(), it->count);
    it->addCount();
//      test.cpp:25:5: error: member function 'addCount' not viable: 'this' argument has
//    type 'const Item', but function is not marked const
    //!! set 的key是不可以改变的，即使不用const_iterator, 默认是const
    // 解决方法，改存指针，而不是值
    it->addCount();

    
    return 0;
}











