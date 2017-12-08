#include "BaseMgr.h"
#include <iostream>

using namespace std;

// std::shared_ptr<BaseMgr>     BaseMgr::s_pInst(new BaseMgr);

BaseMgr::BaseMgr()
{
    cout << "BaseMgr construct" << endl;
}

