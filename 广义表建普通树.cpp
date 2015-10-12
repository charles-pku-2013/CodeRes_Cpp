#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <deque>
#include <cmath>
#include <cassert>
// #include "LOG.h"

using namespace std; 

#define DBG(...)

struct TreeNode {
    TreeNode( const string &_data ) : data(_data) {}

    void addChild( TreeNode *p ) { children.push_back(p); }

    string                      data;
    vector<TreeNode*>       children;
};

set<int> result;
string searchObj;

void Traverse( TreeNode *p, int level )
{
    if( !p ) return;

    // for( int i = 0; i < level; ++i )
        // cout << "\t";
    // cout << p->data << endl;
    
    if( p->data == searchObj )
        result.insert( level );

    for( vector<TreeNode*>::iterator it = p->children.begin();
            it != p->children.end(); ++it ) {
        Traverse( *it, level + 1 );
    } // for 
}


void destroy_tree( TreeNode *p )
{
    if( !p ) return;

    for( vector<TreeNode*>::iterator it = p->children.begin();
            it != p->children.end(); ++it ) {
        destroy_tree( *it );
    } // for

    delete p; 
}


TreeNode* create_tree( const string &str )
{
    TreeNode    *tree = NULL;
    TreeNode    *p;
    stack<TreeNode*>        stk;
    string      nodeStr;
    string::size_type       pos = 0, last_pos = 0;
    
    static const char *OPCHAR = "{},";

    DBG("%s", str.c_str());

    while( (pos = str.find_first_of(OPCHAR, last_pos)) != string::npos ) {
        if( pos != last_pos ) {
            nodeStr.assign( str.begin()+last_pos, str.begin()+pos );
            p = new TreeNode( nodeStr );
            DBG("Create new node %s", p->data.c_str());
            if( !tree ) tree = p;
            if( !stk.empty() ) {
                DBG("Add connect: %s -> %s", stk.top()->data.c_str(), p->data.c_str());
                stk.top()->addChild(p);
            } // if 
        } // if 

        switch( str[pos] ) {
        case '{':
            {
                DBG("push stack: %s", p->data.c_str());
                stk.push( p );
            }
            break;
        case '}':
            {
                DBG("pop stack: %s", stk.top()->data.c_str());
                if( !stk.empty() )
                    stk.pop();
            }
            break;
        case ',':
            {
                break;
            }
        } // switch 

        last_pos = pos + 1;
        if( str.begin()+last_pos == str.end() )
            break;
    } // while


    return tree;
}



int main( int argc, char **argv )
{
    string record;

    while( true ) {
        getline( cin, record );
        if( !cin ) break;
        getline( cin, searchObj );
        if( !cin ) break;
        
        result.clear();
        TreeNode *tree = create_tree( record );
        Traverse( tree, 1 );

        if( result.empty() ) {
            cout << "-1" << endl;
        } else {
            vector<int> resVec( result.begin(), result.end() );
            for( size_t i = 0; i < resVec.size()-1; ++i )
                cout << resVec[i] << ",";
            cout << resVec.back() << endl;
        } // if 

        destroy_tree( tree );
    } // while 

    return 0;
}
