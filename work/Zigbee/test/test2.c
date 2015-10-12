#include "zigbee_common.h"

#define PROFILEID           ZCL_HA_PROFILE_ID
#define DEVICEID            ZCL_HA_DEVICEID_ON_OFF_SWITCH
#define DEVICEVERSION       1
#define ENDPOINT            12

static uint16 outClusters[] = { ZCL_CLUSTER_ID_GEN_ON_OFF };

//!! how to get the dest endpoint
static
void toggle_light( uint16 destAddr )
{
    uint8 buf[] = { 0x01, 0x00, 0x02 };
    uint8 i;
    for( i = 0; i < 10; ++i ) {
        printf( "Sending toggle command, %d\n", i );
        buf[1] = (uint8)i;
//        zigbee_send_data( 0xFFFE, ZCL_CLUSTER_ID_GEN_ON_OFF, (uint8)i, 0, 1, 3, buf );
        zigbee_send_data( destAddr, 13, ENDPOINT, ZCL_CLUSTER_ID_GEN_ON_OFF, i, 0, DEF_RADIUS, 3, buf );
        sleep(3);
    }
    
    return;
}

void test()
{
    uint8 buf[256], *ptr;
    int ret, i;
    
    DBG( "test ZDO_NWK_ADDR_REQ" );
    // get the callback of ZB_FIND_DEVICE_CONFIRM instead of ZDO_NWK_ADDR_RSP
    // fe 0b 46 85 01 b3 5a f0 61 dc 01 00 4b 12 00 35
    // the device's ieee addr
    //!! ZCD_NV_ZDO_DIRECT_CB must be set to 1 first.
    ptr = buf;
    memcpy( ptr, zigbee_dev_info.ieee_addr, 8 );
    ptr += 8;
    *ptr++ = 0;
    *ptr++ = 0;
    ret = zigbee_request( 0x0025, 0x0A, buf, ZIGBEE_INVALID_CMD, NULL, NULL, ZIGBEE_INVALID_CMD, NULL, NULL );
    if( ret )
        DBG( "request error, %d", ret );
}

void test1()
{
	uint8 data[] = { 0x01, 0x2D, 0x00 };
	int ret;

//	int zigbee_send_data( uint16 dstSAddr, uint8 dstep, uint8 srcep, uint16 clusterID, uint8 seqno, uint8 opt,
//						 uint8 radius, uint8 len, void *data )


	ret = zigbee_send_data( 0x00, 0, 12, 0x06, 1, 0, DEF_RADIUS, 3, data );
	if( ret )
		DBG( "Send data error. %d", ret );
}

int main( int argc, char **argv )
{
    int fd;
    uint8 buf[256], *ptr;
    int ret, i;
    uint16 dstAddr;
    
    if( (fd = open_zigbee_device(argv[1], B115200)) < 0 ) {
        perror( "open serial error" );
        exit(-1);
    }
    
    setbuf( stdout, NULL );
    
//    // START
    ret = zigbee_init( 2, 3 );
    if( ret ) {
        DBG( "zigbee init failed, %d.", ret);
        exit(-1);
    }
    
    // register app
    ret = zigbee_app_register( ENDPOINT, PROFILEID, DEVICEID, DEVICEVERSION,
                              0, NULL, ARRLEN(outClusters), outClusters );
    if( ret ) {
        DBG( "register app failed, %d.", ret);
        exit(-1);
    }
    
    // start
    ret = zigbee_start();
    if( ret ) {
        DBG( "start failed, %d.", ret);
        exit( -1 );
    }
    
    WAIT( "Wait a minute for startup finish..." );
    zigbee_get_device_info();      //!! this should be done in the start()
    zigbee_print_device_info();
   
 //   fe 0d 45 c1 08 23 08 23 15 fe 05 02 00 4b 12 00 00 3c /* ZDO_END_DEVICE_ANNCE_IND */
//    WAIT( "Press Enter to start End device binding...");
//    ret = zigbee_end_dev_bind( zigbee_dev_info.shortAddr, zigbee_dev_info.ieee_addr, ENDPOINT, PROFILEID, 0, NULL,
//                        ARRLEN(outClusters), outClusters );
//    if( ret ) {
//        DBG( "End device bind failed, %d.", ret );
//        goto END;
//    }
    
//    WAIT( "Print the binding table ZDO_MGMT_BIND_REQ" );
//    zigbee_print_binding_table(0);
//    
//    printf( "Press Enter to start toggle light test..." );
//    scanf( "%x", &dstAddr );
//    toggle_light( dstAddr );

    
//	test1();
    
    // test read_config
//    buf[0] = 0x83;
//    ret = zigbee_request(0x0426, 1, buf, ZIGBEE_INVALID_CMD, NULL, NULL, ZIGBEE_INVALID_CMD, NULL, NULL );
    

 
END:
    DBG( "read the rest of response..." );
    read_all( fd );
    
    close(fd);
    return 0;
}








