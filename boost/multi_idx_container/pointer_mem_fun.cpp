/*
 * See reference http://www.cs.fsu.edu/~lacher/boost_1_32_0/libs/multi_index/doc/advanced_topics.html
 */
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <boost/format.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>

using namespace boost;
using namespace boost::multi_index;
using namespace std;

class Student {
public:
    explicit Student(uint32_t _Id, const std::string &_Name, char _Gender, uint32_t _Age, double _Score)
                : m_nId(_Id), m_strName(_Name), m_chGender(_Gender), m_nAge(_Age), m_fScore(_Score) {}

    uint32_t id() const { return m_nId; }
    const std::string& name() const { return m_strName; }
    char gender() const { return m_chGender; }
    uint32_t age() const { return m_nAge; }
    double score() const { return m_fScore; }

    void setId(uint32_t id) { m_nId = id; }
    void setName(const std::string &name) { m_strName = name; }
    void setGender(char gender) { m_chGender = gender; }
    void setAge(uint32_t age) { m_nAge = age; }
    void setScore(double score) { m_fScore = score; }

    // for identity
    bool operator < (const Student &rhs) const
    { return id() < rhs.id(); }

    friend std::ostream& operator << (std::ostream &os, const Student &s)
    {
        os << boost::format("%10d\t%-20s\t%c\t%10.2lf") 
                % s.id() % s.name() % s.gender() % s.score();
        return os;
    }

private:
    uint32_t            m_nId;
    std::string         m_strName;
    char                m_chGender;
    uint32_t            m_nAge;
    double              m_fScore;
};

typedef std::shared_ptr<Student>    StudentPtr;


#if 0
// NOTE!!! identity member also work well with pointers
typedef multi_index_container <
    StudentPtr,
    indexed_by <
        // sort by Student::operator<
        ordered_unique< identity<Student> >,
        // sort by less<string> on name
        ordered_non_unique< member<Student, std::string, &Student::m_strName> >,
        // sort by less<int> on score
        ordered_non_unique< member<Student, double, &Student::m_fScore>, std::greater<double> >
    > 
> StudentSet;
#endif

typedef multi_index_container <
    StudentPtr,
    indexed_by <
        // sort by Student::operator <      get<0>
        ordered_unique< identity<Student> >,
        // sort by less<string> on name     get<1>
        ordered_non_unique< const_mem_fun<Student, const std::string&, &Student::name> >,
        // sort by less<int> on score       get<2>
        ordered_non_unique< const_mem_fun<Student, double, &Student::score>, std::greater<double> >
    > 
> StudentSet;

template <int i>
void print_set(const StudentSet &set)
{
    const auto &idx = set.get<i>();
    for (auto it = idx.begin(); it != idx.end(); ++it)
        cout << **it << endl;
}

int main(int argc, char **argv)
{
    StudentSet stuSet;

    uint32_t    id, age;
    string      name, line;
    char        gender;
    double      score;
    while (getline(cin, line)) {
        stringstream stream(line);
        stream >> id >> name >> gender >> age >> score;
        if (stream.fail() || stream.bad())
            continue;
        stuSet.emplace(std::make_shared<Student>(id, name, gender, age, score));
    } // while

    cout << "container size = " << stuSet.size() << endl;
    
    print_set<0>(stuSet);

    cout << endl;
    cout << "Sorted by name: " << endl;
    print_set<1>(stuSet);

    // update name
    {
        auto& nameIdx = stuSet.get<1>();
        auto it = nameIdx.find("Charles");
        if (it != nameIdx.end())
            nameIdx.modify(it, [](StudentPtr &p){p->setName("Charles01");} ); //!! 用修改函数修改
            // NOTE!!! modify_key 只适合修改member修饰的公有变量
            // nameIdx.modify_key(it, [](string &s){s = "Charles01";}); // WRONG
    }
    cout << endl;
    cout << "After update name: " << endl;
    print_set<1>(stuSet);

    // update score thru name
    {
        auto& nameIdx = stuSet.get<1>();
        auto it = nameIdx.find("Lucy");
        if (it != nameIdx.end())
            nameIdx.modify(it, [](StudentPtr &p){p->setScore(100);} ); //!! 正确的方法
            // (*it)->setScore(100);   // WRONG!!! NOTE!!! 用这种方式更新会打乱index
    }
    cout << endl;
    cout << "After update score: " << endl;
    print_set<2>(stuSet);

    return 0;
}

/*
1 Charles f 20 99.885
2 Mary f 25 88.5
3 Lucy f 27 80.75
4 John m 30 77.5
5 Robot m 33 81.5
6 Charles m 25 88.5
 */
