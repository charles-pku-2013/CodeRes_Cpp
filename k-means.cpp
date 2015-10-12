#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <cmath>
// #include "LOG.h"

using namespace std; 

// #define DBG(...)

struct Sample {
    Sample( const size_t _len, const string &s ) : len(_len)
    {
        stringstream    sstr(s);
        data.resize(len);
        for( size_t i = 0; i < len; ++i )
            sstr >> data[i];
        sstr >> classID;
    }

    // data
    vector<double>  data;
    string          classID;
    const size_t    len;
};


struct DistanceSample {
    double      distance;
    Sample      *other;

    bool operator < ( const DistanceSample &rhs ) const 
    { return distance < rhs.distance; }  
};


vector<Sample> trainSet;
vector<Sample> testSet;
int k, l, m, n;


ostream& operator << ( ostream &out, const Sample &s )
{
    copy( s.data.begin(), s.data.end(), ostream_iterator<double>(out, " ") );
    out << " " << s.classID;

    return out;
}


double get_distance( const Sample &lhs, const Sample &rhs )
{
    double sum = 0.0;
    size_t len = ( lhs.len < rhs.len ? lhs.len : rhs.len );
    
    for( size_t i = 0; i < len; ++i ) {
        sum += (lhs.data[i] - rhs.data[i]) * (lhs.data[i] - rhs.data[i]);
    } // for 

    return sqrt( sum );
}


void KNN( Sample &sp )
{
    size_t len = trainSet.size();

    vector<DistanceSample>      distances;
    distances.resize( len );
    
    for( size_t i = 0; i < len; ++i ) {
        distances[i].other = &trainSet[i];
        distances[i].distance = get_distance( sp, trainSet[i] );
    } // for 

    sort( distances.begin(), distances.end() );
    
    size_t n = ( k < len ? k : len );
    map< string, int >      idCount;
    for( int i = 0; i < n; ++i ) {
        ++idCount[ distances[i].other->classID ];
    } // for 

    vector<string> candidateID;
    int max = 0;
    for( map<string,int>::iterator it = idCount.begin(); 
                it != idCount.end(); ++it ) {
        if( it->second >= max ) {
            if( it->second == max ) {
                candidateID.push_back( it->first );
            } else {
                candidateID.clear();
                candidateID.push_back( it->first );
            } // if 
            max = it->second;
        } // if 
    } // for 

    sort( candidateID.begin(), candidateID.end() );

    string finalID;
    for( vector<string>::iterator it = candidateID.begin(); 
            it != candidateID.end(); ++it ) {
        finalID.append( *it );
    } // for 

    sp.classID = finalID;
}


int main( int argc, char **argv )
{
    string line;

    cin >> k >> l >> m >> n;
    getline( cin, line );
    // cout << line << endl;
    // cout << k << l << m << n << endl;
    
    while( m-- ) {
        getline( cin, line );
        Sample s(l, line);
        trainSet.push_back( s );
    } // while 
    
    while( n-- ) {
        getline( cin, line );
        Sample s(l, line);
        testSet.push_back( s );
    } // while

    for( size_t i = 0; i < testSet.size(); ++i ) {
        KNN( testSet[i] );
        cout << testSet[i].classID << endl;
    } // for 



    // cout << testSet[0] << endl << testSet[1] << endl;
    // cout << testSet[1].data.back() << endl;

    // cout << trainSet.size() << endl;
    // cout << get_distance( trainSet[0], trainSet[1] ) << endl;

    return 0;
}
