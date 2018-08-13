#include <iostream>
#include <string>

using namespace std;

void test_int(int &&arg)
{
    cout << "test: " << arg << endl;
}

void test_string(std::string &&arg)
{
    cout << "test: " << arg << endl;
}

int main()
{
    // test_int(5);        // OK
    
    // int i = 5;
    // test_int(i);        // WRONG
    // test_int(std::move(i)); // OK
    // const int& ci = i;
    // test_int(ci);       // WRONG
    // test_int(std::move(ci)); // WRONG would lose const qualifier

    return 0;
}

