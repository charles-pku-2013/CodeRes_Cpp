#include <iostream>
#include "BaseMgr.h"

// std::shared_ptr<BaseMgr>     BaseMgr::s_pInst(new BaseMgr);

int main(int argc, char **argv)
try {
    using namespace std;

    cout << endl;
    const auto &dict = BaseMgr::instance()->getDict();
    for (auto it = dict.begin(); it != dict.end(); ++it)
        cout << it->first << " : " << it->second->id() << endl;
    cout << endl;

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}



