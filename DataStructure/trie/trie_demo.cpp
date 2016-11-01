#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include "trie.hpp"

struct Foo {
    Foo(const std::string &_Name) : name(_Name), age(rand() % 100 + 1) {}

    bool operator<(const Foo &rhs) const
    { return name < rhs.name; }

    friend std::ostream& operator << (std::ostream &os, const Foo &f)
    {
        os << f.name << ":" << f.age;
        return os;
    }

    std::string     name;
    int             age;
};


int main()
{
    using namespace std;

    srand(time(0));

    Trie<Foo> tree;

    string line, word;
    while (getline(cin, line)) {
        stringstream ss(line);
        vector<Foo> arr;
        while (ss >> word)
            arr.emplace_back(word);
        tree.addPath(arr.begin(), arr.end());
    } // while

    tree.traverse(cout);
    cout << endl;

    return 0;    
}


/*
I am studying in Peking University
I am working in Microsoft
I am using Microsoft word
 */
