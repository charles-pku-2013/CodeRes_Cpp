#include <cstdio>
#include <cstring>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

enum { TYPE_DIR, TYPE_FILE };

void rstripString( string &str )
{
    char *start = (char*)(str.c_str());
    char *p = start + str.length() - 1;
    while( p >= start && isspace(*p) )
        *p-- = 0;
}


struct FSTreeNode;
typedef FSTreeNode* FSTreeNodePtr;

struct FSTreeNode {
    struct Comparator {
        bool operator() ( const FSTreeNodePtr pl, const FSTreeNodePtr pr ) const
        { return *pl < *pr; }
    };
    
    typedef set<FSTreeNodePtr, Comparator> ChildrenList;
    typedef ChildrenList::iterator ChildrenIterator;
    typedef pair<ChildrenIterator, bool> ChildrenListInsertRetType;
    
    FSTreeNode( const char *_name, int _type ) : name(_name), type(_type) {}
    
    bool operator < ( const FSTreeNode &rhs ) const
    {
        if( type != rhs.type )
            return type < rhs.type;
        return name < rhs.name;
    }
    
    ChildrenListInsertRetType addChild( const char *name, int type )
    {
        FSTreeNodePtr p = new FSTreeNode(name, type);
        ChildrenListInsertRetType ret = children.insert(p);
        if( !(ret.second) )
            delete p;
        return ret;
    }
    
    string          name;
    int             type;
    ChildrenList    children;
};


class FSTree {
public:
    FSTree() : root(new FSTreeNode("root", TYPE_DIR)) {}
    ~FSTree() { destroyTree(root); }
    
    void addPath( char *str, int type )
    {
        vector<char*> segments;
        for( char *p = strtok(str, "\\"); p; p = strtok(NULL, "\\") )
            segments.push_back(p);
        addPathHelper( root, segments, 0, segments.size(), type );
    }
    
    void printTree() const
    { printHelper(root, 0); }
private:
    void addPathHelper( FSTreeNodePtr p, const vector<char*> &segments, int level, int nLevel, int type )
    {
        if( level >= nLevel )
            return;
        int currentType;
        if( level == nLevel-1 )
            currentType = type;
        else
            currentType = TYPE_DIR;
        FSTreeNode::ChildrenListInsertRetType ret = p->addChild( segments[level], currentType );
        
        FSTreeNodePtr q = *(ret.first);
        addPathHelper( q, segments, level+1, nLevel, type );
    }
    
    void printHelper( FSTreeNodePtr p, int level ) const
    {
        for( int i = 0; i < level*2; ++i )
            printf( " " );
        printf( "%s\n", p->name.c_str() );
        for( FSTreeNode::ChildrenIterator it = p->children.begin(); it != p->children.end(); ++it )
            printHelper( *it, level+1 );
    }
    
    void destroyTree( FSTreeNodePtr p )
    {
        for( FSTreeNode::ChildrenIterator it = p->children.begin(); it != p->children.end(); ++it )
            destroyTree(*it);
        delete p;
    }
private:
    FSTreeNodePtr       root;
};



int main()
{
    int nLines;
    string line;
    int type;
    
    while( scanf( "%d\n", &nLines )==1 ) {
        FSTree T;
        while( nLines-- ) {
            getline(cin, line);
            rstripString(line);
            if( line[line.length()-1] == '\\' )
                type = TYPE_DIR;
            else
                type = TYPE_FILE;
            T.addPath( const_cast<char*>(line.c_str()), type );
        } // while
        T.printTree();
    } // while
    
    return 0;
}
















