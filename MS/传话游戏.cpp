#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#define BLANK " \t\f\r\v\n"

using namespace std;

typedef vector<string> Sentence;
typedef map<string, string> Dictionary;

ostream& operator << ( ostream &os, const Sentence & s )
{
    for( Sentence::const_iterator it = s.begin(); it != s.end()-1; ++it )
        os << *it << " ";
    os << s.back();
    return os;
}

void Replace( string &s, const Dictionary &dict, int nChange )
{
    Dictionary::const_iterator it;
    while( nChange && (it = dict.find(s)) != dict.end() ) {
        s = it->second;
        --nChange;
    } // while
}


void process( Sentence &s, const Dictionary &dict, int nChange )
{
    for( Sentence::iterator it = s.begin(); it != s.end(); ++it )
        Replace( *it, dict, nChange );
}


int main()
{
    int nCase, n, m, nChange;
    char buf1[30], buf2[30];
    string line;
    
    scanf( "%d\n", &nCase );
    for( int c = 1; c <= nCase; ++c ) {
        scanf( "%d %d\n", &n, &m );
        nChange = n - 1;
        Dictionary dict;
        while( m-- ) {
            scanf( "%s %s\n", buf1, buf2 );
            dict[buf1] = buf2;
        } // while
        getline( cin, line );
        Sentence sentence;
        for( char *word = strtok((char*)(line.c_str()), BLANK); word; word = strtok(NULL, BLANK) )
            sentence.push_back( word );
        
        process( sentence, dict, nChange );
        
        cout << "Case #" << c << ": " << sentence << endl;
    } // for
    
    return 0;
}
















