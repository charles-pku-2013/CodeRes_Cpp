#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace boost::multi_index;

struct Employee {
    int            id;
    std::string    name;
    int            age;

    Employee(int _Id, const std::string &_Name, int _Age)
                : id(_Id), name(_Name), age(_Age) {}

    bool operator<(const Employee &rhs) const
    { return id < rhs.id; }

    friend std::ostream& operator<<(std::ostream& os, const Employee& e)
    {
        os << e.id << " " << e.name << " " << e.age;
        return os;
    }
};

struct EmpCmpId {
    // compare an ID and an Employee
    bool operator()(int x,const Employee& e2) const
    { return x < e2.id; }
    // compare an Employee and an ID
    bool operator()(const Employee& e1,int x) const
    { return e1.id < x;}
};

// sorting by name's initial
struct CmpStringInitial {
    bool operator()(char ch, const std::string& s) const
    {
        if (s.empty())
            return false;
        return ch < s[0];
    }

    bool operator()(const std::string& s, char ch) const
    {
        if (s.empty())
            return true;   // '\0' surely less than any ch
        return s[0] < ch;
    }
};

// define a multiply indexed set with indices by id and name
typedef multi_index_container <
    Employee,
    indexed_by <
        // sort by Employee::operator<
        ordered_unique< identity<Employee> >,
        // sort by less<string> on name
        ordered_non_unique< member<Employee, std::string, &Employee::name> >,
        // sort by less<int> on age
        ordered_non_unique< member<Employee, int, &Employee::age> >
    > 
> EmployeeSet;


int main()
{
    using namespace std;

    EmployeeSet es;
    // insert return type: std::pair<iterator,bool>
    es.emplace(Employee(1,"Joe",31));
    es.emplace(Employee(10,"Robert",27));
    es.emplace(Employee(50,"John",40));
    es.emplace(Employee(100,"John",40));
    es.emplace(Employee(120,"Charles",40));
    es.emplace(Employee(180,"Korea",40));

    // lookup by id
    {
        // EmployeeSet::iterator i0 = es.lower_bound(0, EmpCmpId());
        // EmployeeSet::iterator i1 = es.upper_bound(100, EmpCmpId());
        // std::copy(i0, i1, ostream_iterator<Employee>(cout, "\n"));
    }

    // lookup by name
    {
        typedef EmployeeSet::nth_index<1>::type NameIndex;
        NameIndex& idx = es.get<1>();
        NameIndex::iterator i0 = idx.lower_bound('J', CmpStringInitial());
        NameIndex::iterator i1 = idx.lower_bound('K', CmpStringInitial()); // NOTE!!! not upper_bound
        std::copy(i0, i1, ostream_iterator<Employee>(cout, "\n"));
    }

    return 0;
}

