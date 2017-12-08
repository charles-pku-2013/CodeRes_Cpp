#ifndef _BASE_H_
#define _BASE_H_

#include <iostream>
#include <boost/format.hpp>

class Base {
public:
    Base(int _ID) : m_nID(_ID)
    { std::cout << "Base construct" << std::endl; }

    virtual ~Base() {}
    int id() const { return m_nID; }

protected:
    int         m_nID;
};

#endif /* _BASE_H_ */

