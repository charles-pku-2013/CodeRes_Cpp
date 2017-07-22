/*
 * c++ -o /tmp/test build_variadic_args.cpp -std=c++11 -ljsoncpp -g
 */
#include <json/json.h>
#include <iostream>
#include <utility>

using namespace std;

template <typename Key, typename Value>
void build_json(Json::Value &jv, const Key &key, const Value &val)
{
    jv[key] = val;
}

template <typename Key, typename Value, typename... Args>
void build_json(Json::Value &jv, const Key &key, const Value &val, Args&&... rest)
{
    build_json(jv, key, val);
    build_json(jv, rest...);
}


template <typename ValueType>
void build_json_pair(Json::Value &jv, const ValueType &val)
{
    jv[val.first] = val.second;
}

template <typename ValueType, typename... Args>
void build_json_pair(Json::Value &jv, const ValueType &val, Args&&... rest)
{
    build_json_pair(jv, val);
    build_json_pair(jv, rest...);
}

int main()
{
    Json::Value conf;
    // build_json(conf, "msg", "test message",
                     // "status", 0,
                     // "path", "/usr/bin");

    build_json_pair(conf, std::make_pair("msg", "test message"),
                          std::make_pair("status", 0),
                          std::make_pair("path", "/home/charles"));

    cout << Json::StyledWriter().write(conf) << endl;

    return 0;
}


