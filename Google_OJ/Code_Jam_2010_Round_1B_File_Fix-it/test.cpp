#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>

struct FSTreeNode;
typedef FSTreeNode* FSTreeNodePtr;
struct FSTreeNodePtrComparator;


//!! 内部类大显用武之地! 若Comparator定义为外部类，则互相引用 incomplete type
struct FSTreeNode {
    struct Comparator {
        bool operator() ( const FSTreeNodePtr pl, const FSTreeNodePtr pr ) const
        { return pl->name < pr->name; }
    };
    
    typedef std::set<FSTreeNodePtr, Comparator>::iterator Iterator;
    typedef std::pair<Iterator, bool> InsertRetType;
    
    FSTreeNode() {}
    FSTreeNode( const char *_name ) : name(_name) {}
    
    InsertRetType addChild( const char *name )
    {
        FSTreeNodePtr p = new FSTreeNode(name);
        InsertRetType ret = children.insert(p);
        if( !(ret.second) )
            delete p;
        return ret;
    }
    
    std::string name;
    std::set<FSTreeNodePtr, Comparator> children;
};


class FSTree {
public:
    FSTree() : root(new FSTreeNode("#")) {}
    ~FSTree() { delTree(root); }
    
    int addPath( char *str )
    {
        std::vector<char*> segments;
        for( char *p = strtok(str, "/"); p; p = strtok(NULL, "/") )
            segments.push_back(p);
        return addPathHelper(root, segments, 0);
    }
private:
    int addPathHelper( FSTreeNodePtr p, const std::vector<char*> &seg, int index )
    {
        if( index >= seg.size() )
            return 0;
        int retval = 0;
        FSTreeNode::InsertRetType ret = p->addChild( seg[index] );
        if( ret.second )
            ++retval;
        FSTreeNodePtr q = *(ret.first);
        retval += addPathHelper(q, seg, index+1);
        return retval;
    }
    
    void delTree( FSTreeNodePtr p )
    {
        for( FSTreeNode::Iterator it = p->children.begin(); it != p->children.end(); ++it )
            delTree(*it);
        delete p;
    }
private:
    FSTreeNodePtr       root;
};




int main()
{
    using namespace std;
    
    int nCases, nExists, nTests;
    string line;
    
/*    // test
    {
        FSTree tree;
        int ret;
        char str[] = "/a/b/c", str1[] = "/a/b/d";
        ret = tree.addPath(str);
        printf( "%d\n", ret );
        ret = tree.addPath(str1);
        printf( "%d\n", ret );
        exit(0);
    }
*/
    
    scanf( "%d\n", &nCases );
    for( int c = 1; c <= nCases; ++c ) {
        FSTree tree;
        scanf( "%d %d\n", &nExists, &nTests );
        while( nExists-- ) {
            getline(cin, line);
            tree.addPath(const_cast<char*>(line.c_str()));
        } // while
        
        int sum = 0;
        while( nTests-- ) {
            getline(cin, line);
            sum += tree.addPath(const_cast<char*>(line.c_str()));
        } // while
        
        printf( "Case #%d: %d\n", c, sum );
    } // for
    
    return 0;
}







