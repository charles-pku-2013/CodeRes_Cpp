#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <climits>

#define MAX (INT_MAX/2)

using namespace std;

typedef vector<int> GraphRow;

// int matrix
struct Graph : public vector<GraphRow> {
    Graph( int n ) : vector<GraphRow>(n)
    {
        for( int i = 0; i < n; ++i )
            (*this)[i].resize(n, MAX);
    }
    
    void addEdge( int start, int end, int weight )
    { (*this)[start][end] = weight; }
};


struct DijRecord {
    DijRecord() : finish(false), distance(MAX) {}
    
    void addToPath( int n ) { path.push_back(n); }
    
    bool            finish;
//    int             node;
    int             distance;
    list<int>       path;
};


void DijShortest( Graph &g, int s )
{
    int n = g.size();
    
    // 初始化工作向量
    vector<DijRecord> record(n);
    for( int i = 0; i < n; ++i ) {
        record[i].distance = g[s][i];
    } // for
    record[s].finish = true;
    
    do {
        int k = -1, min = MAX;
        for( int i = 0; i < n; ++i ) {
            if( !(record[i].finish) && record[i].distance < min ) {
                min = record[i].distance;
                k = i;
            } // if
        } // for
        
        if( k == -1 ) break;
        record[k].finish = true;
        
        for( int i = 0; i < n; ++i ) {
            int newDist = record[k].distance + g[k][i];
            if( !(record[i].finish) && newDist < record[i].distance ) {
                record[i].distance = newDist;
                record[i].path = record[k].path;    //!! 特别注意路径处理
                record[i].addToPath(k);         //!! 不是简单添加
            } // if
        } // for
    } while( true );
    
    // print
    for( int i = 0; i < n; ++i ) {
        if( i == s ) continue;
        printf( "%d -> %d:%d:\t", s, i, record[i].distance );
        for( list<int>::const_iterator it = record[i].path.begin(); it != record[i].path.end(); ++it )
            printf("%d ", *it);
        putchar('\n');
    } // for
}


int main()
{
    int n, m;
    int start, end, dist;
    
    while( scanf("%d %d\n", &n, &m) == 2 ) {
        Graph g(n);
        while( m-- ) {
            scanf( "%d %d %d\n", &start, &end, &dist );
            g.addEdge( start, end, dist );
        } // while
        DijShortest( g, 0 );
    } // while
    
    return 0;
}



















