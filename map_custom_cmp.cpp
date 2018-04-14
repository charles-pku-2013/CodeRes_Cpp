#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

enum RouterStatInterval {HOUR, MINUTE};

struct TimeStamp {
    TimeStamp() : hour(0), minute(0) {}
    TimeStamp(int _Hour, int _Minute) : hour(_Hour), minute(_Minute) {}

    int hour, minute;
};

struct TimeStampCmp {
    TimeStampCmp(int _Field) : field(_Field) {}

    bool operator()(const TimeStamp &lhs, const TimeStamp &rhs) const
    {
        if (field == HOUR) 
        { return lhs.hour < rhs.hour; } 
        return lhs.minute < rhs.minute;
    }

    int field;
};



int main()
{
    int interval = HOUR;
    std::map<TimeStamp, uint32_t, TimeStampCmp>   statTable((TimeStampCmp(interval))); // NOTE!!! 变量必须放在括号里
    TimeStamp tm(1,2);
    // ++statTable[tm];
    cout << statTable.size() << endl;
    
    return 0;
}
