#ifndef _FOO_H_
#define _FOO_H_

#include <map>
#include <string>
#include <iostream>

class Foo {
public:
    static bool init();

private:
    static std::map<std::string, int>   m_dict;
};

#endif /* _FOO_H_ */

