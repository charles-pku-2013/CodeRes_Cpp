#ifndef _BASE_H_
#define _BASE_H_

class Base {
public:
    Base(int _ID);
    virtual ~Base() {}
    int id() const { return m_nID; }
protected:
    int         m_nID;
};

#endif /* _BASE_H_ */

