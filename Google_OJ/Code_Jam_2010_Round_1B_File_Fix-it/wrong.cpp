#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>

template <typename T>
void delArray( T &container )
{
    for( typename T::iterator it = container.begin(); it != container.end(); ++it )
    delete *it;
}

class Path {
public:
    Path() {}
    Path( char *str)
    {
        for( char *p = strtok(str, "/"); p; p = strtok(NULL, "/") )
            pathSegments.push_back(p);
    }
    
    size_t getLevel() const
    { return pathSegments.size(); }
    
    std::string &operator[]( size_t index )
    { return pathSegments[index]; }
    const std::string &operator[]( size_t index ) const
    { return pathSegments[index]; }
    
    bool operator< ( const Path &rhs ) const
    {
        if( this->getLevel() != rhs.getLevel() )
            return this->getLevel() < rhs.getLevel();
        
        for( int i = 0; i < getLevel(); ++i ) {
            if( (*this)[i] != rhs[i] )
                return (*this)[i] < rhs[i];
        } // for
        
        return false;
    }
    
    int getDiff( const Path &rhs ) const
    {
        int diff = 0, level, i;
        if( this->getLevel() > rhs.getLevel() ) {
            level = rhs.getLevel();
            diff += this->getLevel() - rhs.getLevel();
        } else {
            level = this->getLevel();
        }
        
        for( i = 0; i < level; ++i ) {
            if( (*this)[i] != rhs[i] )
                break;
        } // for
        diff += level - i;
        
        return diff;
    }
private:
    std::vector<std::string>    pathSegments;
};

typedef Path* PathPtr;


struct PathPtrComparator {
    bool operator() ( const PathPtr pl, const PathPtr pr ) const
    { return *pl < *pr; }
};


typedef std::set<PathPtr, PathPtrComparator> PathSet;
typedef std::pair<PathSet::iterator, bool> PathSetInsertRetType;


PathSet pathSet;

// for debug
void printPath( const Path &p )
{
    int i;
    for( i = 0; i < p.getLevel()-1; ++i )
        printf( "/%s", p[i].c_str() );
    printf( "/%s\n", p[i].c_str() );
}

void printAll()
{
    for( PathSet::iterator it = pathSet.begin(); it != pathSet.end(); ++it )
        printPath(**it);
}


int compute( PathPtr p )
{
    int retVal, diff;
    
    printf( "COMPUTE path: " );
    printPath(*p);
    
    PathSetInsertRetType ret = pathSet.insert(p);
    if( !(ret.second) ) {
        delete p;
        return 0;
    }
    
    if( pathSet.size() == 1 )
        return p->getLevel();
    
    PathSet::iterator cur = ret.first;
    if( cur != pathSet.begin() ) {
        PathSet::iterator prev = cur;
        --prev;
        retVal = p->getDiff(**prev);
    }
    PathSet::iterator next = cur;
    ++next;
    if( next != pathSet.end() ) {
        diff = p->getDiff(**next);
        if( diff < retVal )
            retVal = diff;
    } // if
    
    printf( "NOW the pathSet is:\n" );
    printAll();
    printf( "retVal = %d\n", retVal );
    
    return retVal;
}


int main()
{
    using namespace std;
    
    int nCases, nExists, nTests;
    string line;
    
    scanf( "%d\n", &nCases );
    for( int c = 1; c <= nCases; ++c ) {
        pathSet.clear();
        scanf( "%d %d\n", &nExists, &nTests );
        while( nExists-- ) {
            getline(cin, line);
            pathSet.insert( new Path( const_cast<char*>(line.c_str()) ) );
        } // while
        
        int sum = 0;
        while( nTests-- ) {
            getline(cin, line);
            PathPtr p = new Path( const_cast<char*>(line.c_str()) );
            sum += compute(p);
        } // while
        
        printf( "Case #%d: %d\n", c, sum );
      
        delArray(pathSet);
    } // for
    
    return 0;
}

















