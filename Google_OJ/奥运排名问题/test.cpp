#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

#define MAX 65535

using namespace std;

enum { CMP_BY_GOLD=1, CMP_BY_TOTAL, CMP_BY_AVG_GOLD, CMP_BY_AVG_TOTAL };

struct Country {
    Country() {}
    Country( int _NO, int _nGold, int _nTotal, int _population )
        : NO(_NO), nGold(_nGold), nTotal(_nTotal), population(_population)
    {}
    
    int         NO;
    int         nGold;
    int         nTotal;
    int         population;
};

typedef Country* CountryPtr;

struct CountryComparator {
    CountryComparator( int _cmpMethod ) : cmpMethod(_cmpMethod) {}
    
    bool operator() ( const CountryPtr pl, const CountryPtr pr ) const
    {
        switch(cmpMethod) {
        case CMP_BY_GOLD:
            return pl->nGold > pr->nGold;
        case CMP_BY_TOTAL:
            return pl->nTotal > pr->nTotal;
        case CMP_BY_AVG_GOLD:
            return (float)(pl->nGold)/pl->population > (float)(pr->nGold)/pr->population;
        } // switch
        
        return (float)(pl->nTotal)/pl->population > (float)(pr->nTotal)/pr->population;
    }
    
    int cmpMethod;
};


struct Query {
    Query( int _no ) : countryNO(_no), rank(MAX) {}
    int countryNO;
    int cmpMethod;
    int rank;
};

typedef Query* QueryPtr;

//!! 最好将这些东西做成数组，与enum关联
vector<CountryPtr>          sortedGold;
vector<CountryPtr>          sortedTotal;
vector<CountryPtr>          sortedAvgGold;
vector<CountryPtr>          sortedAvgTotal;

template <typename T>
void delVector( T &container )
{
    for( typename T::iterator it = container.begin(); it != container.end(); ++it )
        delete *it;
}


void buildTable( const vector<CountryPtr> &records )
{
    sortedGold.assign( records.begin(), records.end() );
    sort( sortedGold.begin(), sortedGold.end(), CountryComparator(CMP_BY_GOLD) );
    sortedTotal.assign( records.begin(), records.end() );
    sort( sortedTotal.begin(), sortedTotal.end(), CountryComparator(CMP_BY_TOTAL) );
    sortedAvgGold.assign( records.begin(), records.end() );
    sort( sortedAvgGold.begin(), sortedAvgGold.end(), CountryComparator(CMP_BY_AVG_GOLD) );
    sortedAvgTotal.assign( records.begin(), records.end() );
    sort( sortedAvgTotal.begin(), sortedAvgTotal.end(), CountryComparator(CMP_BY_AVG_TOTAL) );
}


int getRank( int countryNO, const vector<CountryPtr> &sorted, int cmpMethod )
{
    vector<CountryPtr>::const_iterator it;
    int rank = 0;
    for( it = sorted.begin(); it != sorted.end(); ++it ) {
        ++rank;
        if( (*it)->NO == countryNO )
            break;
    } // for
    
    CountryComparator compare(cmpMethod);
    CountryPtr ptr = *it;
    --it;
    for( ; it >= sorted.begin(); --it ) {
        if( !compare(ptr, *it) && !compare(*it, ptr) )      //!! ==
            --rank;
    } // for
    
    return rank;
}


void runQuery( vector<QueryPtr> &queryItems )
{
    QueryPtr p;
    int rank;
    for( vector<QueryPtr>::iterator it = queryItems.begin(); it != queryItems.end(); ++it ) {
        p = *it;
        for( int i = CMP_BY_GOLD; i <= CMP_BY_AVG_TOTAL; ++i ) {
            switch(i) {
            case CMP_BY_GOLD:
                rank = getRank( p->countryNO, sortedGold, i );
                break;
            case CMP_BY_TOTAL:
                rank = getRank( p->countryNO, sortedTotal, i );
                break;
            case CMP_BY_AVG_GOLD:
                rank = getRank( p->countryNO, sortedAvgGold, i );
                break;
            case CMP_BY_AVG_TOTAL:
                rank = getRank( p->countryNO, sortedAvgTotal, i );
                break;
            } // switch
            if( rank < p->rank ) {
                p->rank = rank;
                p->cmpMethod = i;
            } // if
        } // for
        printf( "%d:%d\n", p->rank, p->cmpMethod );
    } // for
    putchar('\n');
}


int main()
{
    int nCountry, nQuery;
    int cNO, gold, total, population;
    vector<CountryPtr> records;
    vector<QueryPtr> queryItems;
    
    while( scanf("%d %d\n", &nCountry, &nQuery)==2 ) {
        records.resize(nCountry, NULL);
        for( int i = 0; i < nCountry; ++i ) {
            cNO = i;
            scanf( "%d %d %d\n", &gold, &total, &population );
            records[i] = new Country(cNO, gold, total, population);
        } // for
        
        buildTable( records );
        
        queryItems.resize(nQuery, NULL);
        for( int i = 0; i < nQuery; ++ i ) {
            scanf( "%d", &cNO );
            queryItems[i] = new Query(cNO);
        } // for

        runQuery( queryItems );
        
        delVector(records);
        delVector(queryItems);
        getchar();
    } // while
    
    return 0;
}
















