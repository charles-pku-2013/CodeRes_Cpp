#include <iostream>
#include <string>
#include <boost/format.hpp>

using namespace std;

int main()
{
    string name = "Charles";
    int age = 20;
    double score = 98.1415926;    

    cout << boost::format("Student %s whose age is %d whose score is %0.2f")
            % name.c_str() % age % score << endl;

    return 0;
}

