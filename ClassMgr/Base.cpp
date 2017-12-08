#include "Base.h"
#include "BaseMgr.h"
#include <iostream>

using namespace std;

/*
 * NOTE!!! 应该放在Mgr所管理基类的cpp文件中，Base的构造函数也应该放在这里而不是在头文件。
 * 确保当第一个子类构造并注册前，基类构造函数的执行从而导致Mgr的初始化。
 */
std::shared_ptr<BaseMgr>     BaseMgr::s_pInst(new BaseMgr);

Base::Base(int _ID) : m_nID(_ID) 
{ cout << "Base construct" << endl; }
