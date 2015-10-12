#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <unistd.h>

#define PATH "/etc/dict.dat"

using namespace std;

typedef map<string, string> Dictionary;

static
Dictionary dict;

static
void print()
{
//    printf( "dictionary:\n" );
    for( Dictionary::iterator it = dict.begin(); it != dict.end(); ++it )
        printf( "%s = %s\n", it->first.c_str(), it->second.c_str() );
}

static
void wrong_usage()
{
	printf( "usage:\n" );
	printf( "\tnvram get key\n" );
	printf( "\tnvram set key value\n" );
	exit(-1);
}

static
bool load_dict()
{
    ifstream ifs( PATH );
    if( !ifs ) {
        //printf( "cannot open %s for reading\n", PATH );
        return false;
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> dict;
    return true;
}

static
bool save_dict()
{
    ofstream ofs( PATH );
    if( !ofs ) {
        //printf( "cannot open %s for writting\n", PATH );
        return false;
    }
    boost::archive::text_oarchive oa(ofs);
    oa << dict;
#ifdef ROOTFS_IN_RAM   
    ofs.close();
    system( "save_file.sh /etc/dict.dat" );
#endif   
    return true;
}

static
void get_helper( const string &key )
{
    if( !load_dict() ) {
        putchar( '\n' );
        return;
    }

    Dictionary::iterator it = dict.find( key );
    if( it == dict.end() )
        putchar( '\n' );
    else printf( "%s\n", (it->second).c_str() );

    return;
}

static
void set_helper( const string &key, const string &value )
{
	load_dict();

	Dictionary::iterator it = dict.find( key );
	if( it != dict.end() )
		it->second = value;
	else
		dict.insert( it, Dictionary::value_type(key, value) );

	save_dict();

	return;
}

int main( int argc, char **argv )
{
	if( argc < 3 )
		wrong_usage();
	
	if( strcmp( argv[1], "get" ) == 0 ) {
		get_helper( argv[2] );
	}
	else if( strcmp( argv[1], "set" ) == 0 ) {
		if( argc < 4 )
			wrong_usage();		
		set_helper( argv[2], argv[3] );
#if 0
		if( strcmp(argv[2], "TZ") == 0 ) {
			pid_t pid = getppid();
			printf( "!!!!!!!!!NOTICE!!!!!!!! process %d is setting the TIMEZONE!\n", pid );
			char cmd[40];
//			sprintf( cmd, "ps -ef | grep \'%d\'", pid );
			sprintf( cmd, "pstree -p" );
			system( cmd );
		}
#endif
	} else if( strcmp(argv[1], "show") == 0 && strcmp(argv[2], "all") == 0 ) {
		load_dict();
	 	print();
	} else {
		wrong_usage();
	}

	return 0;
}
















