/*
 * c++ -o /tmp/test heterogeneous_lookup.cpp -std=c++14 -g
 */
#include <iostream>
#include <memory>
#include <set>
#include <boost/format.hpp>

using namespace std;


struct Employee {
    const std::string     name;
    int             age;
    double          salary;

    friend std::ostream& operator << (std::ostream &os, const Employee &ee)
    {
        os << boost::format("[Employee: %s, age = %d, salary = %lf]") % ee.name % ee.age % ee.salary;
        return os;
    }
};

typedef std::shared_ptr<Employee>   EmployeePtr;


void test1()
{
    struct Cmp {
        using is_transparent = std::true_type;  // 🔴🔴 必须有 backward compatibility
        bool operator()(const EmployeePtr &lhs, const EmployeePtr &rhs) const
        { return lhs->name < rhs->name; }
        bool operator()(const std::string &lhs, const EmployeePtr &rhs) const
        { return lhs < rhs->name; }
        bool operator()(const EmployeePtr &lhs, const std::string &rhs) const
        { return lhs->name < rhs; }
        bool operator()(const char *lhs, const EmployeePtr &rhs) const
        { return strcmp(lhs, rhs->name.c_str()) < 0; }
        bool operator()(const EmployeePtr &lhs, const char *rhs) const
        { return strcmp(lhs->name.c_str(), rhs) < 0; }
    };

    std::set<EmployeePtr, Cmp>  empSet;
    empSet.emplace(std::make_shared<Employee>(Employee{"Lucy", 25, 100.5}));
    auto it = empSet.find("Lucy");
    if (it != empSet.end())
        cout << **it << endl;
    else
        cout << "Not found!" << endl;
}


int main()
{
    test1();

    return 0;
}


