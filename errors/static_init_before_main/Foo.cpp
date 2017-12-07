#include "Foo.h"

/*
 * NOTE!!! 放在这里也不行，必须在m_dict定义之后 
 */
// static bool _dummy_init = Foo::init();

std::map<std::string, int>   Foo::m_dict;

static bool _dummy_init = Foo::init();

bool Foo::init()
{
    std::cout << "Foo::init()" << std::endl;

    m_dict["charles"] = 10;
    m_dict["mary"] = 20;
    m_dict["Lucy"] = 30;

    return true;
}
