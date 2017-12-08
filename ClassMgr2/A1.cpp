#include "A1.h"
#include "BaseMgr.h"
#include <iostream>

using namespace std;

/*
 * NOTE!!! Segmentation fault!!!
 * 似乎在main函数之前是按照字典序初始化全局变量的
 */
static bool _dummy_register = BaseMgr::instance()->add("A1", std::make_shared<A1>(11));

A1::A1(int id) : Base(id)
{ cout << "A1 " << id << " construct." << endl; }

