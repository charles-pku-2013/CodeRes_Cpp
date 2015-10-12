#include "LOG.h"
#include <cstdio>
#include <sstream>



using namespace std;

struct Foo {
	void print()
	{ int i = 5; DBG_COND( i > 3, "In function Foo::print() %ld", (long)this); }
};


int
main(int argc, char **argv)
{
	int i = 5;
	DBG_STREAM_COND( i > 3, "test dbg " << i );
	// DBG( "%d", i );

	Foo f;
	f.print();	// cannot print class name qualifier
	
	return 0;
}



/* 
int
main(int argc, char **argv)
{
 int i = 5;
 do { 
	std::stringstream __dbg_stream_stringstream; 
	__dbg_stream_stringstream << "test.cpp" << ":" 9 << " in function: " << __func__ << "(): " << "test dbg" << i; printf("%s\n", __dbg_stream_stringstream.str().c_str()); 
 } while(0);

 return 0;
}
 */