//!! 图着色问题
// https://code.google.com/codejam/contest/2933486/dashboard
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <bitset>
#include <queue>

#define N_COLORS 2

enum { UNCOLORED, RED, BLUE };

struct GraphNode {
    GraphNode() : color(UNCOLORED) {}
    GraphNode( const char *_name ) : name(_name), color(UNCOLORED) {}
    
    bool operator < ( const GraphNode &rhs ) const
    { return name < rhs.name; }
    
    bool dye( const std::bitset<N_COLORS+1> &colorset )
    {
        using namespace std;
        // dbg
//        cout << name << " dying...............";
//        cout << colorset << endl;
        
        if( !((~colorset).any()) )
            return false;
        
        for( int i = 1; i <= N_COLORS; ++i ) {
            if( !colorset[i] ) {
                color = i;
                return true;
            }
        } // for
        
        return false;
    }
    
    std::string name;
    int color;
};

typedef GraphNode* GraphNodePtr;

struct GraphNodeComparator {
    bool operator() ( const GraphNodePtr pl, const GraphNodePtr pr ) const
    { return *pl < *pr; }
};

//!! 图结构定义
typedef std::set<GraphNodePtr, GraphNodeComparator> AdjSet;
typedef std::pair<AdjSet::iterator, bool> AdjSetInsertRetType;
typedef std::map<GraphNodePtr, AdjSet, GraphNodeComparator> Graph;
typedef std::pair<Graph::iterator, bool> GraphInsertRetType;


//void printGraph( const Graph &g )
//{
//    using namespace std;
//    
//    for( Graph::const_iterator it = g.begin(); it != g.end(); ++it ) {
//        cout << it->first->name << ": ";
//        const AdjSet &adj = it->second;
//        for( AdjSet::const_iterator it1 = adj.begin(); it1 != adj.end(); ++it1 )
//            cout << (*it1)->name << " ";
//        cout << endl;
//    } // for
//}


class RelationGraph : public Graph {
public:
    void addRelation( const char *s1, const char *s2 )
    {
        GraphNodePtr p1, p2;
        iterator it1, it2;
        GraphInsertRetType gInsertRet;
//        AdjSetInsertRetType aInsertRet;
        
        p1 = new GraphNode(s1);
        gInsertRet = this->insert( Graph::value_type(p1, AdjSet()) );
        it1 = gInsertRet.first;
        if( !(gInsertRet.second) ) {
            delete p1;
            p1 = it1->first;
        } // if
        
        p2 = new GraphNode(s2);
        gInsertRet = this->insert( Graph::value_type(p2, AdjSet()) );
        it2 = gInsertRet.first;
        if( !(gInsertRet.second) ) {
            delete p2;
            p2 = it2->first;
        } // if
        
        it1->second.insert(p2);
        it2->second.insert(p1);
    }
    
    ~RelationGraph()
    {
        for( iterator it = this->begin(); it != this->end(); ++it )
            delete it->first;
    }
};

bool process( RelationGraph &g )
{
    using namespace std;
    
    queue<GraphNodePtr> Q;
    for( RelationGraph::iterator it = g.begin(); it != g.end(); ++it ) {
        GraphNodePtr p = it->first;
        if( p->color == UNCOLORED ) {
            Q.push(p);
            while( !Q.empty() ) {
                GraphNodePtr q = Q.front();
                Q.pop();
                AdjSet &adj = g[q];
                bitset<N_COLORS+1> colorset;
                colorset.set(UNCOLORED);
                // 看邻接节点用了哪些颜色, 还有哪些颜色可用
                for( AdjSet::iterator ait = adj.begin(); ait != adj.end(); ++ait ) {
                    GraphNodePtr r = *ait;
                    if( r->color == UNCOLORED )
                        Q.push(r);
                    else
                        colorset.set(r->color);
                } // for
                bool colorable = q->dye(colorset);
                if( !colorable ) return false;
            } // while !Q.empty()
        } // if
    } // for
    
    return true;
}

int main()
{
    int nCases, nItems;
    char s1[80], s2[80];
    
    scanf( "%d\n", &nCases );
    for( int c = 1; c <= nCases; ++c ) {
        RelationGraph g;
        scanf( "%d\n", &nItems );
        while( nItems-- ) {
            scanf( "%s %s\n", s1, s2 );
            g.addRelation( s1, s2 );
        } // while
//        printGraph(g);
        bool ret = process(g);
        printf( "Case #%d: %s\n", c, ret ? "Yes" : "No" );
    } // for
    
    return 0;
}














