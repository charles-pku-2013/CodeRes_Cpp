#include <cstdio>
#include <cstring>


struct Student {
    char        name[40];
    int         age;
    char        ID[11];
    float       GPA;
};

// By removing the '&', see the difference
void func( Student &a )
{
    printf( "func: the addr of arg is %x\n", &a );
    printf( "a.age = %d\n", a.age );
    a.age += 10;
    printf( "now a.age = %d\n", a.age );
    printf( "func over!\n" );
}



int main()
{
    Student stu;
    
    printf( "%d\n", sizeof(stu) );
    printf( "%x\n", &stu );
    
    strcpy( stu.name, "Wang Jing" );
    stu.age = 24;
    strcpy( stu.ID, "1301221193" );
    stu.GPA = 3.5;
    
    func( stu );
    
    printf( "%d\n", stu.age );
    
    return 0;
}







