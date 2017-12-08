#ifndef _BASE_MGR_H_
#define _BASE_MGR_H_

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <boost/format.hpp>
#include "Base.h"

class BaseMgr {
public:
    typedef std::shared_ptr<Base>  BasePtr;
public:
    ~BaseMgr()
    { std::cout << boost::format("BaseMgr destruct %lx") % (void*)this << std::endl; }

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

    /*
     * NOTE!!! 最优解决方案，singleton最佳实现方式
     */
    static BaseMgr* instance()
    { 
        static std::shared_ptr<BaseMgr> pInst(new BaseMgr);
        return pInst.get();
    }

private:
    BaseMgr()
    { std::cout << boost::format("BaseMgr construct %lx") % (void*)this << std::endl; }

    std::map<std::string, BasePtr>   m_dictBase;
};


#endif /* _BASE_MGR_H_ */

