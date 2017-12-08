#include "A2.h"
#include "BaseMgr.h"
#include <iostream>

using namespace std;

static bool _dummy_register = BaseMgr::instance()->add("A2", std::make_shared<A2>(14));

A2::A2(int id) : Base(id)
{ cout << "A2 " << id << " construct." << endl; }

