#ifndef _ZIGBEE_COMMON_H
#define _ZIGBEE_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <termios.h>

// types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

#ifdef BIG_ENDIAN
    #define BYTESWAP(ptr) do {										\
                            uint8 *p1 = (uint8*)(ptr);				\
                            uint8 *p2 = p1 + (sizeof(*(ptr))-1);	\
                            do {									\
                                SWAP( *p1, *p2 );					\
                            } while( ++p1 < --p2 );					\
                        } while(0)
#else
    #define BYTESWAP
#endif

#define read_all( fd ) do { 				\
			uint8 data;						\
			ssize_t n;					\
			while( (n = read(fd, &data, 1)) == 1 )	\
				printf( "%02x ", data );	\
			if( n < 0 )						\
				perror( "read error!" );	\
			printf( "n = %d\n" );			\
		}while(0)

#define WAIT( str ) do {                    \
            printf( "%s\n", str );          \
            getchar();                      \
        } while(0)

#define print_mem( ptr, len ) do { 			\
			int i;							\
			for( i = 0; i < len; ++i )		\
				printf( "%02x ", *((uint8*)(ptr) + i) );		\
			putchar('\n');					\
		}while(0)

// DEBUG
#ifdef _DEBUG
	#define DBG( fmt, ... ) fprintf( stdout, fmt"\n", ##__VA_ARGS__ )
	#define pr_mem( ptr, len ) print_mem( ptr, len )
#else
	#define DBG
	#define pr_mem
#endif

#define ZIGBEE_TIMEOUT 10
#define ZIGBEE_BUFLEN 256

// structures
typedef struct {
    uint8       state;
    uint16      shortAddr;
    uint8       ieee_addr[8];
    uint16      panID;
    uint8       extPanID[8];
    uint8       channel;
    uint16      parentSAddr;
    uint8       parentIeeeAddr[8];
} ZigbeeDeviceInfo_t;

#define DEF_RADIUS                      30
/* commands */
#define ZIGBEE_INVALID_CMD              0x0
// nv items
#define ZCD_NV_ZDO_DIRECT_CB			0x008F		// enable callback funcs
#define SYS_OSAL_NV_READ_REQ			0x0821
#define SYS_OSAL_NV_READ_RSP 			0x0861
#define SYS_OSAL_NV_WRITE_REQ			0x0921
#define SYS_OSAL_NV_WRITE_RSP			0x0961
#define SYS_OSAL_NV_ITEM_INIT_REQ		0x0721
#define SYS_OSAL_NV_ITEM_INIT_RSP		0x0761
#define ZCD_NV_STARTUP_OPTION           0x0003
#define NV_ITEM_UNINIT					0x09
// system
#define SYS_RESET_REQ					0x0041
#define SYS_RESET_CB					0x8041
// ZDO
#define ZDO_END_DEVICE_BIND_REQ         0x2025
#define ZDO_END_DEVICE_BIND_RSP         0x2065
#define ZDO_END_DEVICE_BIND_CB          0xA045

// nv_configid
#define ZCD_NV_STARTUP_OPTION			0x0003	
#define ZCD_NV_LOGICAL_TYPE 			0x0087
#define ZCD_NV_PANID        			0x0083
#define ZCD_NV_CHANLIST					0x0084

/* register and start */
// register
#define AF_REGISTER_REQ                 0x0024
#define AF_REGISTER_RSP                 0x0064
// start
#define ZDO_STARTUP_FROM_APP_REQ        0x4025
#define ZDO_STARTUP_FROM_APP_RES        0x4065      //!! the value 1 doesn't mean error!! redesign
#define AF_DATA_REQ                     0x0124
#define AF_DATA_RES                     0x0164
#define AF_DATA_CB                      0x8044

/* SAPI */
// register
#define ZB_APP_REGISTER_REQUEST_REQ			0x0A26
#define ZB_APP_REGISTER_REQUEST_RSP			0x0A66
// start
#define ZB_START_REQ                        0x0026
#define ZB_START_RSP                        0x0066
#define ZB_START_CB                         0x8046
// data transfer
#define ZB_SEND_DATA_REQ                    0x0326
#define ZB_SEND_DATA_RSP                    0x0366
#define ZB_SEND_DATA_CB                     0x8346      // ZB_SEND_DATA_CONFIRM
// get device info
#define ZB_GET_DEVICE_INFO_REQ              0x0626
#define ZB_GET_DEVICE_INFO_RSP              0x0666
#define ZB_DEVICE_INFO_STATE                0
#define ZB_DEVICE_INFO_IEEE_ADDR            1
#define ZB_DEVICE_INFO_SHORT_ADDR           2
#define ZB_DEVICE_INFO_PARENT_SHORT_ADDR    3
#define ZB_DEVICE_INFO_PARENT_IEEE_ADDR     4
#define ZB_DEVICE_INFO_CHANNEL              5
#define ZB_DEVICE_INFO_PANID                6
#define ZB_DEVICE_INFO_EXT_PANID            7
// ZDO commands
#define ZDO_NODE_DESC_REQ                   0x0225
#define ZDO_NODE_DESC_RSP                   0x0265
#define ZDO_NODE_DESC_CB                    0x8245
#define ZDO_MGMT_BIND_REQ                   0x3325
#define ZDO_MGMT_BIND_CB                    0xB345

// profile ids
// ha
#define ZCL_HA_PROFILE_ID             	0x0104
// devices
#define ZCL_HA_DEVICEID_ON_OFF_SWITCH  	0x0000
// cluster ids
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG              0x0007

/*** Foundation Command IDs ***/
#define ZCL_CMD_READ                                    0x00
#define ZCL_CMD_READ_RSP                                0x01
#define ZCL_CMD_WRITE                                   0x02
#define ZCL_CMD_WRITE_UNDIVIDED                         0x03
#define ZCL_CMD_WRITE_RSP                               0x04
#define ZCL_CMD_WRITE_NO_RSP                            0x05
#define ZCL_CMD_CONFIG_REPORT                           0x06
#define ZCL_CMD_CONFIG_REPORT_RSP                       0x07
#define ZCL_CMD_READ_REPORT_CFG                         0x08
#define ZCL_CMD_READ_REPORT_CFG_RSP                     0x09
#define ZCL_CMD_REPORT                                  0x0a
#define ZCL_CMD_DEFAULT_RSP                             0x0b
#define ZCL_CMD_DISCOVER                                0x0c
#define ZCL_CMD_DISCOVER_RSP                            0x0d


// global vars declaration
extern ZigbeeDeviceInfo_t zigbee_dev_info;
extern int zigbee_fd;

// util func declaration
extern ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timout);
extern ssize_t twrite(int fd, const void *buf, size_t nbytes, unsigned int timout);
extern ssize_t treadn(int fd, void *buf, size_t nbytes, unsigned int timout);
extern ssize_t writen(int fd, const void *ptr, size_t n);
//extern ssize_t twriten(int fd, const void *buf, size_t nbytes, unsigned int timout);
extern int open_zigbee_device( const char *dev_name, int baudrate );
extern int zigbee_request( uint16 reqid, uint8 reqlen, void *reqdata, 
		uint16 rspid, uint8 *rsplen, void *rspdata,
		uint16 callbackid, uint8 *callbacklen, void *callbackdata );

// cmds
extern int zigbee_init( uint8 type, uint8 clearOpt );
extern int zigbee_reset();
extern int read_nv_config( uint16 configid, uint8 *length, void *value );
extern int write_nv_config( uint16 configid, uint8 length, void *value );
extern int zigbee_app_register( uint8 endpoint, uint16 profileid, uint16 devid,
                               uint8 devvVer, uint8 nInCluster, uint16 *pInClusters,
                               uint8 nOutCluster, uint16 *pOutClusters );
extern int zigbee_start();
extern void zigbee_get_device_info();
extern int zigbee_end_dev_bind( uint16 coordSAddr, uint8 *coordEAddr, uint8 endpoint, uint16 profileid,
                               uint8 nInCluster, uint16 *pInClusters, uint8 nOutCluster, uint16 *pOutClusters );
//extern int zigbee_send_data( uint16 dstSAddr, uint16 clusterID, uint8 seqno, uint8 ack,
//                            uint8 radius, uint8 len, void *data );
extern int zigbee_send_data( uint16 dstSAddr, uint8 dstep, uint8 srcep, uint16 clusterID, uint8 seqno, uint8 opt,
                            uint8 radius, uint8 len, void *data );
// utils for serial port
extern int OpenSerial( int fd, const char* port);
extern int InitSerial(int fd,	int speed, int flow_ctrl, int databits, int stopbits, int parity);

// others
extern uint16 to_u16( void *ptr );
extern uint32 to_u32( void *ptr );

// for testing
extern void zigbee_print_device_info();
extern int zigbee_print_binding_table( uint16 addr );

#endif








