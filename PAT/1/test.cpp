#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

#define N_SORT_TYPE     4
#define EPSILON         0.000001

using namespace std;

enum { GOLD, TOTAL, AVGGOLD, AVGTOTAL };

inline
bool floatEqual( float a, float b )
{ return fabs(a-b) < EPSILON; }


struct Country {
    Country( int _id, int _gold, int _total, int _population )
        : id(_id), gold(_gold), total(_total), population(_population)
    {
        avgGold = (float)gold / population;
        avgTotal = (float)total / population;
    }
    
    int id;
    int gold;
    int total;
    int population
    float   avgGold;
    float   avgTotal;
};


typedef Country* CountryPtr;

struct CountryComparator {
    CountryComparator( int _method ) : method(_method) {}
    
    bool operator() ( const CountryPtr pl, const CountryPtr pr ) const
    {
        switch(method) {
            case TOTAL:
                return pl->total < pr->total;
            case AVGGOLD: {
                if( !floatEqual(pl->avgGold, pr->avgGold) )
                    return pl->avgGold < pr->avgGold;
                return false;
            }
            case AVGTOTAL: {
                if( !floatEqual(pl->avgTotal, pr->avgTotal) )
                    return pl->avgTotal < pr->avgTotal;
                return false;
            }
        } // switch
        
        return pl->gold < pr->gold;
    }
    
    int method;
};


vector<CountryPtr> sorted[N_SORT_TYPE];

inline
void resizeSorted( int nCountry )
{
    for( int i = 0; i < N_SORT_TYPE; ++i )
        sorted[i].resize( nCountry, NULL );
}



int main()
{

    
    return 0;
}










