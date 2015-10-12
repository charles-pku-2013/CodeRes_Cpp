#include "zigbee_common.h"

#if 0
void test1(  )
{
	uint8 *ptr;
	uint8 checksum;
	uint16 length;
	ssize_t n;
	
	// atinfo
//	uint8 buf[] = { 0x02, 0x0, 0x07, 0x0, 'a', 't', 'i', 'n', 'f', 'o', 0x77, 0x03 };
	// remote atinfo
//	uint8 buf[] = { 0x02, 0x00, 0x09, 0x83, 0x2E, 0xD7, 0x61, 0x74, 0x69, 0x6E, 0x66, 0x6F, 0, 0x03 };
	// ATEPREQ
//	uint8 buf[] = { 0x02, 0x00, 0x0A, 0x83, 0x2E, 0xD7, 'a', 't', 'e', 'p', 'r', 'e', 'q', 0, 0x03 };
	// voltage detect
	// 02 00 12 85 2e d7 00 00 01 04 04 fd 08 04 01 00 00 00 21 4e 04 dd 03 
	uint8 buf[] = { 0x02, 0x00, 0x0E, 0x85, 0x2E, 0xD7, 0x00, 0x00, 0x01, 0x04, 0x04, 0xFD, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x03 };

	// get length
	length = (uint16)((buf + sizeof(buf) - 2) - (buf + 3));
	BYTESWAP( &length );
	memcpy( buf+1, &length, 2 );

	// get checksum
	ptr = buf + (sizeof(buf) - 2);
	checksum = get_checksum( buf + 1, ptr );
	*ptr = checksum;
	
	pr_mem(buf, sizeof(buf));
	n = writen( zigbee_fd, buf, sizeof(buf) );
	if( n != sizeof(buf) )
		DBG( "Write error!" );
}
#endif

#if 0
void test1()
{
	uint8 buf[] = { 0x01, 0x00, 0x02 };
	int ret;
	uint16 dstAddr = 0x2ED7;
	ret = zigbee_send_zcl_cmd( dstAddr, 0, 0, ZCL_HA_PROFILE_ID, ZCL_CLUSTER_ID_GEN_ON_OFF,
			3, buf );
	if( ret )
		DBG( "zigbee_send_zcl_cmd fail, %d", ret );
}
#endif

//void test2()
//{
//	int ret;
////	ret = zigbee_at_cmd( API_ATCMD, NULL, "atinfo", NULL );
////	if( ret )
////		DBG( "zigbee_at_cmd error: %d", ret );

////	uint8 buf[] = { 0x11, 0x01, 0x02 };

//	Address_t addr;
//	make_address( &addr, SHORT, 0x2ED7 );
////	ret = zigbee_zcl_cmd( &addr, 0x0, 0x0, 0x0104, 0x0006, buf, sizeof(buf) );
//	ret = zigbee_zcl_cmd( &addr, 0x0, 0x0, 0x0104, 0x0006, 
//		ZCL_FRAME_CLUSTER_SPEC | ZCL_FRAME_NO_RSP, 0x01, 1 );
//	if( ret )
//		DBG( "zigbee_zcl_cmd error: %d", ret );
//}

//static
//void printThisDevice()
//{
//	char buf[20];

//	printf( "This device info:\n" );
//	printf( "Channel:\t\t\t%u\n", thisDev.channel );
//	printf( "PAN ID:\t\t\t%04x\n", thisDev.panId );
//	switch( thisDev.type ) {
//	case COORD:
//		strcpy( buf, "Coord" );
//		break;
//	case ROUTER:
//		strcpy( buf, "Router" );
//		break;
//	case ENDDEV:
//		strcpy( buf, "End Device" );
//		break;		
//	} // switch	
//	printf( "Device Type:\t\t\t%s\n", buf );
//	printf( "Address:\t\t\t%04x\n", thisDev.addr );
//	printf( "MAC Addr:\t\t\t" );
//	print_mem( thisDev.macAddr, 8 );
//	printf( "P2P Addr:\t\t\t%04x\n", thisDev.p2pAddr );
//	printf( "Security:\t\t\t%s\n", thisDev.security ? "ON" : "OFF" );
//}

int main( int argc, char **argv )
{
    uint8 buf[256], *ptr;
    int ret, i;
    uint16 dstAddr;
    
//    if( (fd = open_zigbee_device(argv[1])) < 0 ) {
//        perror( "open serial error" );
//        exit(-1);
//    }

//    if( (fd = open("/dev/null", O_WRONLY)) < 0 ) {
//        perror( "open serial error" );
//        exit(-1);
//    }
//	zigbee_fd = fd;


    setbuf( stdout, NULL );
	ret = zigbee_init( argv[1], 115200,0,8,1,'N' );
	if( ret ) {
		DBG( "zigbee_init error: %d", ret );
		exit(-1);
	}	

	thisDev.print();
	
	testUtil();
    
END: 

	WAIT( "Press Enter to end..." );
	zigbee_finalize();
    return 0;
}








