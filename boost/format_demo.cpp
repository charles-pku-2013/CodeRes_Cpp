#include <iostream>
#include <string>
#include <boost/format.hpp>

using namespace std;

int main()
{
    string name = "Charles";
    int age = 20;
    double score = 98.1415926;
    double x = 3.14159;
    // double x = 3.0;

    cout << boost::format("Student %s whose age is %d whose score is %0.2f")
            % name.c_str() % age % score << endl;

    std::string s = boost::str(boost::format("Student %s whose age is %d whose score is %0.2f")
            % name.c_str() % age % score);

    cout << boost::format("x = %0.lf") % x << endl;
    // cout 不会输出3.0
    cout << "x = " << x << endl;

    return 0;
}

