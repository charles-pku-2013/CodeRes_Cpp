#include <cstdio>
#include <cstdlib>
#include <climits>
#include <string>
#include <functional>
#include <algorithm>
#include <deque>
#include <list>
#include <vector>

#define INFINITY (INT_MAX/2)

using namespace std;

class GraphNode;

struct AdjElem {
    AdjElem( GraphNode *node, int w ) : adjNode(node), weight(w) {}
    GraphNode       *adjNode;
    int             weight;
};

typedef list<AdjElem> AdjList;

class GraphNode {
public:
    GraphNode( int no ) : NO(no), inDegree(0) {}
    int getNO() const { return NO; }
    int getOutDegree() const { return adjList.size(); }
    void addAdj( const AdjElem &adj )
    { adjList.push_back(adj); }
    void addInDegree() { ++inDegree; }
    int decInDegree() { return --inDegree; }
    int getInDegree() const { return inDegree; }
    AdjList& getAdjList() { return adjList; }   
    const AdjList& getAdjList() const { return adjList; }
protected:
    int         NO;
    int         inDegree;
    AdjList     adjList;
};

typedef GraphNode* GraphNodePtr;

class Graph {
public:
    typedef vector<GraphNodePtr>    AdjTable;
    typedef vector< vector<int> >   AdjMatrix;
public:
    Graph( int n_nodes ) : nNodes(n_nodes)
    {
        adjTable.resize(nNodes, NULL);
        adjMatrix.resize(nNodes);
        for( int i = 0; i < nNodes; ++i )
            adjMatrix[i].resize(nNodes, INFINITY);
        for( int i = 0; i < nNodes; ++i )
            adjMatrix[i][i] = 0;
    }

    ~Graph()
    {
        for( int i = 0; i < nNodes; ++i )
            delete adjTable[i];
    }

    void addEdge( int start, int end, int weight )
    {
        if( !adjTable[start] )
            adjTable[start] = new GraphNode(start);
        if( !adjTable[end] )
            adjTable[end] = new GraphNode(end);

        adjTable[start]->addAdj( AdjElem(adjTable[end], weight) );
        adjTable[end]->addInDegree();

        adjMatrix[start][end] = weight;
    }

    int numOfNodes() const { return nNodes; }
    AdjTable& getAdjTable() { return adjTable; }    
    const AdjTable& getAdjTable() const { return adjTable; }
    AdjMatrix& getAdjMatrix() { return adjMatrix; }
    const AdjMatrix& getAdjMatrix() const { return adjMatrix; }

    void printAdjMatrix() const;
    bool topoSort( vector<GraphNodePtr> &result ) const;
protected:
    int                             nNodes;
    AdjTable                        adjTable;
    AdjMatrix                       adjMatrix;
};


bool Graph::topoSort( vector<GraphNodePtr> &result ) const
{
    deque<GraphNodePtr>         workq;
    vector<bool>                visited(nNodes, false);
    int                         nValidNodes = 0;

    result.clear();
    for( int i = 0; i < nNodes; ++i ) {
        if( adjTable[i]  ) {
            ++nValidNodes;
            if( adjTable[i]->getInDegree() == 0 )
                workq.push_back(adjTable[i]);
        } // if
    } // for

    if( workq.empty() )
        return false;

    while( !workq.empty() ) {
        GraphNodePtr p = workq.front();
        workq.pop_front();
        result.push_back(p);
        visited[p->getNO()] = true;

        for( AdjList::iterator it = p->getAdjList().begin(); it != p->getAdjList().end(); ++it ) {
            int vex = it->adjNode->getNO();
            if( !visited[vex] && adjTable[vex]->decInDegree() == 0 )
                workq.push_back(adjTable[vex]);
        } // for
    } // while

//  for( vector<GraphNodePtr>::iterator it = result.begin(); it != result.end(); ++it )
//      printf( "%d\t", (*it)->getNO() );
//  putchar('\n');

    return result.size() == nValidNodes;
}


void Graph::printAdjMatrix() const
{
    for( int i = 0; i < nNodes; ++i ) {
        for( int j = 0; j < nNodes; ++j ) {
            if( INFINITY == adjMatrix[i][j] )
                printf( "X\t" );
            else
                printf( "%d\t", adjMatrix[i][j] );
        }
        putchar('\n');
    }
}


struct FloydElem {
    int weight;
    list<int> path;
    void addPath( int i )
    { path.push_back(i); }
    void addPath( const list<int> &p )
    { path.insert( path.end(), p.begin(), p.end() ); }
};


void Floyd( Graph &g )
{
    typedef vector< vector<FloydElem> > FloydMatrix;

    int n = g.numOfNodes();
    int i, j, k;
    FloydMatrix D(n);
    Graph::AdjMatrix &M = g.getAdjMatrix();

    // initial D(-1)
    for( i = 0; i < n; ++i ) {
        D[i].resize(n);
        for( j = 0; j < n; ++j )
            D[i][j].weight = M[i][j];
    } // for

    for( k = 0; k < n; ++k ) {
        for( i = 0; i < n; ++i ) {
//          if( k == i )
//              continue;
            for( j = 0; j < n; ++j ) {
                if( /*k != j &&*/ D[i][j].weight > D[i][k].weight + D[k][j].weight ) {
                    D[i][j].weight = D[i][k].weight + D[k][j].weight;
                    D[i][j].addPath(D[i][k].path);
                    D[i][j].addPath(k);
                    D[i][j].addPath(D[k][j].path);
                } // if
            } // for j
        } // for i
    } // for k

    // print result
    for( i = 0; i < n; ++i ) {
        for( j = 0; j < n; ++j ) {
            if( j != i && D[i][j].weight != INFINITY ) {
                printf( "%d -> ", i );
                for( list<int>::iterator it = D[i][j].path.begin(); it != D[i][j].path.end(); ++it )
                    printf( "%d -> ", *it );
                printf( "%d: %d\n", j, D[i][j].weight );
            } // if
        } // for j
    } // for i
}


int main()
{
    int i, j, w, n;
    int nNodes = 0;
    vector<int> vec;

    while(1) {
        n = scanf( "%d %d %d\n", &i, &j, &w );
        if( -1 == i )
            break;
        if( n != 3 ) {
            printf( "Invalid input!\n" );
            exit(-1);
        }

        if( i > nNodes ) nNodes = i;
        if( j > nNodes ) nNodes = j;

        vec.push_back(i);
        vec.push_back(j);
        vec.push_back(w);
    } // while

    Graph g(++nNodes);
    for( int index = 0; index < vec.size(); index += 3 )
        g.addEdge(vec[index], vec[index+1], vec[index+2]);


    printf( "The adj-Matrix of the graph is:\n" );
    g.printAdjMatrix();


    vector<GraphNodePtr> topoSortRes;
    if( g.topoSort( topoSortRes ) ) {
        printf( "One of topo-sort sequences in this graph is:\n" );
        for( vector<GraphNodePtr>::iterator it = topoSortRes.begin(); it != topoSortRes.end(); ++it )
            printf( "%d\t", (*it)->getNO() );
        putchar('\n');
    } else {
        printf( "There does not exist a valid topo-sort sequence in this graph.\n" );
    }

    printf( "Shortest path between every pair of nodes is:\n" );
    Floyd(g);
    
    return 0;
}


// Graph Data 1
/*
1 0 7
1 2 6
0 2 1
2 3 5
3 1 2
-1 -1 -1
*/

// Graph Data 2
/*
0 1 10
0 2 2
2 1 3
-1 -1 -1
*/

