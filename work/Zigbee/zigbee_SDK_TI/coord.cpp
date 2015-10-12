#include "zigbee.h"


//typedef boost::shared_ptr<int> IntPtr;

//struct IntPtrFinder {
//	IntPtrFinder( int _data ) : data(_data) {}

//	bool operator() ( const IntPtr &ptr ) const
//	{ return *ptr == data; }

//	int data;
//};


//static
//void testBasic()
//{
//	using namespace std;

//#ifdef _DEBUG
//	zigbeelog1 = fopen( "/tmp/zigbeeLog1", "w" );
//	setbuf( zigbeelog1, NULL ); 
//	zigbeelog2 = fopen( "/tmp/zigbeeLog2", "w" );
//	setbuf( zigbeelog2, NULL ); 
//#endif

//	// CallbackSafeList
//	{
//		ThreadSafePtrList<IntPtr> list;
//		list.add( IntPtr(new int(5)) );
//		IntPtr p = list.find_remove( IntPtrFinder(5) );
//	}


//	return;
//}


int main( int argc, char **argv )
{
	using namespace std;

	int ret;
	uint8 type = 0;
	uint8 clearOpt = 0;

	if( argc < 2 ) {
		printf( "usage: test devname roletype clearOpt\n" );
		exit(-1);
	}

	if( argc >= 3 )
		type = (uint8)atoi(argv[2]);
	if( argc >=4 )
		clearOpt = (uint8)atoi(argv[3]);

	if( type ) {
		zigbeelog1_filename = "/tmp/zigbeeEnddevLog1";
		zigbeelog2_filename = "/tmp/zigbeeEnddevLog2";		
	} else {
		zigbeelog1_filename = "/tmp/zigbeeCoordLog1";
		zigbeelog2_filename = "/tmp/zigbeeCoordLog2";
	}

	zigbee_role_type = type;
	
//	testBasic();
//	return 0;
	
	
	ret = zigbee_init( argv[1], type, clearOpt, 115200, 1, 8, 1, 'N' );
	if( ret != 0) {
		printf( "zigbee init error %s\n", strerror(errno) );
		exit(-1);
	}

	setbuf( stdout, NULL );

//	sleep(1);
//	zigbee_scan();

//	sleep(1);
//	zigbee_set_link_key();
//	WAIT( "Press Enter to go on." );

//	sleep(1);
//	zigbee_start();	
	
	testUtil();

	WAIT( "Press Enter to end test." );
	goto END;


END:
	printf( "zigbee test end!\n" );
	zigbee_finalize();
	return 0;
}







