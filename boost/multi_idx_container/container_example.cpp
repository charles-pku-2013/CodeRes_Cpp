#include <string>
#include <iostream>
#include <functional>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>

using namespace boost;
using namespace boost::multi_index;
using namespace std;

struct Employee{
    int            id;
    std::string    name;
    int            age;

    Employee(int _Id, const std::string &_Name, int _Age)
                : id(_Id),name(_Name),age(_Age) {}

    friend std::ostream& operator<<(std::ostream& os, const Employee& e)
    {
        os << e.id << " " << e.name << " " << e.age << std::endl;
        return os;
    }
};

// boost::multi_index::multi_index_container
// You can also choose hashed_non_unique
typedef multi_index_container <
    Employee,
    indexed_by <
        ordered_unique<member<Employee, int, &Employee::id> >,
        ordered_non_unique<member<Employee, string, &Employee::name>, std::greater<std::string> >,
        ordered_non_unique<member<Employee, int, &Employee::age> >
    >
> EmployeeContainer;

typedef EmployeeContainer::nth_index<0>::type IdIndex;
typedef EmployeeContainer::nth_index<1>::type NameIndex;
typedef EmployeeContainer::nth_index<2>::type AgeIndex;

int main()
{
    EmployeeContainer con;
    // insert return type: std::pair<iterator,bool>
    con.insert(Employee(0,"Joe",31));
    con.insert(Employee(1,"Robert",27));
    con.insert(Employee(2,"John",40));
    con.insert(Employee(3,"John",40));

    IdIndex& ids = con.get<0>();   // iterator container
    copy(ids.begin(),ids.end(), ostream_iterator<Employee>(cout));
    cout << endl;

    // find
    {
        auto it = ids.find(1);
        if (it != ids.end())
            cout << "found!" << endl;
        else
            cout << "not found!" << endl;
        // it->age = 80; // error: assignment of member ‘Employee::age’ in read-only object
        // it = ids.erase(it);
        // cout << *it << endl;
    }

    NameIndex& names = con.get<1>();
    copy(names.begin(), names.end(), ostream_iterator<Employee>(cout));
    cout << endl;

    // find & modify
    {
        auto it = names.find("Joe");
        if (it != names.end()) {
            cout << "found " << it->name << endl;
            // 下面两者等价 below 2 have equal effect.
            // names.modify_key(it, [](string &s){s = "Charles";});
            names.modify(it, [](Employee &e){e.name = "Charles";} );
            // 也可以修改其他的key
            names.modify(it, [](Employee &e){e.age = 10;} );
        } // if
    }

    names.erase(names.begin());

    AgeIndex& ages = con.get<2>();
    copy(ages.begin(), ages.end(), ostream_iterator<Employee>(cout));
    cout << endl;

    return 0;
}
