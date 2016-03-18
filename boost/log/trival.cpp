// compile: c++ -o /tmp/test trival.cpp -DBOOST_LOG_DYN_LINK -lboost_log -lpthread -g
// compile(C++11): c++ -o /tmp/test trival.cpp -DBOOST_LOG_DYN_LINK -std=c++11 -pthread -lboost_log  -g


#include <boost/log/trivial.hpp>

int main(int, char*[])
{
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return 0;
}
