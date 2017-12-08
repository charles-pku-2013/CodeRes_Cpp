#include "Derived1.h"
#include "BaseMgr.h"
#include <iostream>

using namespace std;

static bool _dummy_register = BaseMgr::instance()->add("Derived1", std::make_shared<Derived1>(10));

Derived1::Derived1(int id) : Base(id)
{ cout << "Derived1 " << id << " construct." << endl; }

