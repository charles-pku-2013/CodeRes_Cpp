#include <iostream>
#include <glog/logging.h>
#include "absl/strings/str_format.h"
#include <boost/lexical_cast.hpp>

int main() {
    std::string s("0.33103448");
    float f = boost::lexical_cast<float>(s);
    LOG(ERROR) << f;
    return 0;
}

