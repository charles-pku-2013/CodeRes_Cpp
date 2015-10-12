#include <cstdio>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include <iostream>

using namespace std;

struct Foo {
	Foo( const int _val ) : val(_val)
	{ printf("Foo constructor %ld\n", (long)this); }
	
	~Foo()
	{ printf("Foo destructor %ld\n", (long)this); }
	
	operator int () const
	{ return val; }
	
	int val;
};


int main(int argc, char **argv)
{
	boost::shared_ptr<Foo> p;

	if(!p)
		cout << "empty ptr" << endl;
	
	p.reset(new Foo(5));
	cout << *p << endl;
	
	// p.release();	//!! ‘class boost::shared_ptr<int>’ has no member named ‘release’
	// p.reset();	//!! this works! clear a shared_ptr
	p.reset( (Foo*)NULL );	//!! also works!!
	
	if(!p)
		cout << "empty ptr" << endl;
	
	return 0;
}





