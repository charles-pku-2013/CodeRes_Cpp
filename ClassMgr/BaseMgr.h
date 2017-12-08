#ifndef _BASE_MGR_H_
#define _BASE_MGR_H_

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include "Base.h"

class BaseMgr {
public:
    typedef std::shared_ptr<Base>  BasePtr;
public:
    BaseMgr();

    bool add(const std::string &name, const BasePtr &ptr)
    { 
        std::cout << "Adding " << name << std::endl;
        m_dictBase[name] = ptr; 
        return true;
    }

    bool getByName(const std::string &name, BasePtr &ptr)
    {
        auto it = m_dictBase.find(name);
        if (it == m_dictBase.end())
            return false;
        ptr = it->second;
        return true;
    }

    const std::map<std::string, BasePtr>& getDict() const
    { return m_dictBase; }

    static std::shared_ptr<BaseMgr> instance()
    { return s_pInst; }

private:
    std::map<std::string, BasePtr>   m_dictBase;
    static std::shared_ptr<BaseMgr>     s_pInst;
};


#endif /* _BASE_MGR_H_ */

