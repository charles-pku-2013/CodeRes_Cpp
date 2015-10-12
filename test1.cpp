#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <climits>

#define MAX (INT_MAX/2)

using namespace std;

struct GraphElem {
    GraphElem() : distance(MAX), cost(MAX) {}
    GraphElem( int _dist, int _cost ) : distance(_dist), cost(_cost) {}
    
    int         distance;
    int         cost;
    
    GraphElem operator + ( const GraphElem &rhs ) const
    { return GraphElem( distance + rhs.distance, cost + rhs.cost ); }
    
    bool operator < ( const GraphElem &rhs ) const
    {
        int diff = distance - rhs.distance;
        if( diff ) return diff < 0;
        return cost < rhs.cost;
    }
};


typedef vector<GraphElem>       GraphRow;


struct Graph : public vector<GraphRow> {
    typedef vector<GraphRow> BaseType;
    
    Graph( int n ) : BaseType(n)
    {
        for( int i = 0; i < n; ++i )
            (*this)[i].resize(n);
    }
    
    void addEdge( int a, int b, int d, int p )
    {
        GraphElem elem(d, p);
        (*this)[a][b] = (*this)[b][a] = elem;
    }
};


struct Record {
    Record() : finish(false) {}
    Record( const GraphElem &rhs ) : edge(rhs), finish(false) {}
    
    GraphElem   edge;
    bool        finish;
};


pair<int, int>
ShortestPath( Graph &g, int s, int t )
{
    if( s == t ) return make_pair(0, 0);
    
    int n = g.size()-1;
    vector<Record>      record( g[s].begin(), g[s].end() );
    record[s].finish = true;
    
    do {
        GraphElem min;
        int k = -1;
        for( int i = 1; i <= n; ++i ) {
            if( !(record[i].finish) ) {
                if( record[i].edge < min ) {
                    min = record[i].edge;
                    k = i;
                } // if <
            } // if
        } // for
        
        if( k == -1 ) break;
        record[k].finish = true;
        
        for( int j = 1; j <= n; ++j ) {
            GraphElem newElem = record[k].edge + g[k][j];
            if( !(record[j].finish) && newElem < record[j].edge ) {
                record[j].edge = newElem;
            } // if
        } // for
    } while( true );
    
    return make_pair( record[t].edge.distance, record[t].edge.cost );
}


int main()
{
//    {
//        GraphElem e1(5,6);
//        GraphElem e2(6,3);
//        cout << (e1 < e2) << endl;
//        return 0;
//    }
    
    
    int n, m;
    int a, b, d, p;
    int s, t;
    
    while( scanf("%d %d\n", &n, &m) == 2 ) {
        if( !n && !m ) break;
        Graph g(n+1);
        while( m-- ) {
            scanf( "%d %d %d %d\n", &a, &b, &d, &p );
            g.addEdge( a, b, d, p );
        } // while m
        scanf( "%d %d\n", &s, &t );
        pair<int, int> ret = ShortestPath( g, s, t );
        printf( "%d %d\n", ret.first, ret.second );
    } // while
    
    return 0;
}



















