#include "Derived2.h"
#include "BaseMgr.h"
#include <iostream>

using namespace std;

static bool _dummy_register = BaseMgr::instance()->add("Derived2", std::make_shared<Derived2>(12));

Derived2::Derived2(int id) : Base(id)
{ cout << "Derived2 " << id << " construct." << endl; }

