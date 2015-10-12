#include "zigbee_common.h"

ZigbeeDeviceInfo_t zigbee_dev_info;
int zigbee_fd;

//static helper func dec
static 
int init_nv_config( uint16 configid, uint8 length, void *value );

//!! Endianess relevant
inline
uint16 to_u16( void *ptr )
{
	return *((uint16*)ptr);
}

inline
uint32 to_u32( void *ptr )
{
	return *((uint32*)ptr);
}

ssize_t tread(int fd, void *buf, size_t nbytes, unsigned int timout)
{
	int				nfds;
	fd_set			readfds;
	struct timeval	tv;

	tv.tv_sec = timout;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	nfds = select(fd+1, &readfds, NULL, NULL, &tv);
	if (nfds <= 0) {
		if (nfds == 0)
			errno = ETIME;
		return(-1);
	}
	return(read(fd, buf, nbytes));
}

ssize_t twrite(int fd, const void *buf, size_t nbytes, unsigned int timout)
{
	int				nfds;
	fd_set			writefds;
	struct timeval	tv;

	tv.tv_sec = timout;
	tv.tv_usec = 0;
	FD_ZERO(&writefds);
	FD_SET(fd, &writefds);
	nfds = select(fd+1, &writefds, NULL, NULL, &tv);
	if (nfds <= 0) {
		if (nfds == 0)
			errno = ETIME;
		return(-1);
	}
	return(write(fd, buf, nbytes));
}

ssize_t treadn(int fd, void *buf, size_t nbytes, unsigned int timout)
{
	size_t	nleft;
	ssize_t	nread;

	nleft = nbytes;
	while (nleft > 0) {
		if ((nread = tread(fd, buf, nleft, timout)) < 0) {
			if (nleft == nbytes)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount read so far */
		} else if (nread == 0) {
			break;          /* EOF */
		}
		nleft -= nread;
		buf += nread;
	}
	return(nbytes - nleft);      /* return >= 0 */
}

ssize_t writen(int fd, const void *ptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;

	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) < 0) {
			if (nleft == n)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount written so far */
		} else if (nwritten == 0) {
			break;
		}
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n - nleft);      /* return >= 0 */
}

//ssize_t twriten(int fd, const void *buf, size_t nbytes, unsigned int timout)
//{
//	size_t	nleft;
//	ssize_t	nwrite;
//
//	nleft = nbytes;
//	while (nleft > 0) {
//        if ((nwrite = twrite(fd, buf, nleft, timout)) < 0) {
//            if (nleft == nbytes)
//                return(-1); /* error, return -1 */
//            else
//                break;      /* error, return amount read so far */
//        } else if (nwrite == 0) {
//            break;          /* EOF */
//        }
//        nleft -= nwrite;
//        buf += nwrite;
//	}
//	return(nbytes - nleft);      /* return >= 0 */
//}

// Non-Canonical not text based
int open_zigbee_device( const char *dev_name, int baudrate )
{
	int fd;

	fd = OpenSerial(fd, dev_name);
	if( fd < 0 )
		return fd;

	if( InitSerial(fd,115200,1,8,1,'N') < 0 ) {
		perror( "InitSerial error!" );
		return -1;
	}	  

//#if 0	
//	struct termios newtio;
//
//	fd = open( dev_name, O_RDWR | O_NOCTTY | O_NDELAY ); 
//	if (fd < 0 )
//		return fd;
//
//	// set owner
//	fcntl(fd, F_SETFL, 0);	//??
//
//	tcgetattr( fd, &newtio ); /* save current port settings */
//	//??
//	cfsetispeed(&newtio, baudrate); 
//	cfsetospeed(&newtio, baudrate);
//
////	memset(&newtio, 0, sizeof(newtio));
//	newtio.c_cflag |=  CRTSCTS | CS8 | CLOCAL | CREAD;
//	newtio.c_cflag &= ~CSIZE;		//??
//	newtio.c_cflag &= ~CSTOPB;		//??
//	newtio.c_cflag &= ~PARENB; 		//??
//	newtio.c_iflag = IGNPAR;		//??
//	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
//	newtio.c_oflag  &= ~OPOST;   /*Output*/
////	newtio.c_oflag = 0;
////	newtio.c_lflag = 0;
//	newtio.c_cc[VTIME]	  = 0;
//	newtio.c_cc[VMIN]	  = 1; //!! cannot be set to 0, otherwise, read will return immediately with retval 0
//
//	tcflush(fd, TCIFLUSH);
//	tcsetattr(fd,TCSANOW,&newtio);
//#endif

	zigbee_fd = fd;

	return fd;
}

static
int make_zigbee_frame( uint16 cmd, uint8 datalen, void *data, void *buf )
{
	void *ptr, *q;
	uint8 tmp;
	
	ptr = buf;

	// write SOF
	tmp = 0xFE;
	memcpy( ptr++, &tmp, 1 );
	// write datalen
	memcpy( ptr++, &datalen, 1 );

	// write cmd
	memcpy( ptr, &cmd, 2 );
	ptr += 2;

	// write data
	if( data ) {
		memcpy( ptr, data, datalen );
		ptr += datalen;
	}

	// compute FCS
	tmp = 0;
	for( q = buf + 1; q != ptr; ++q )
		tmp ^= *(uint8*)q;
	memcpy( ptr, &tmp, 1 );

	return datalen + 5;
}

static inline
uint16 *find_in_array( uint16 value, uint16 *ptr, uint8 len )
{
	uint8 i;
	for( i = 0; i < len; ++i )
		if( value == *(ptr + i) )
			return (ptr + i );
	return NULL;
}

static
int get_response( uint16 *rspidlist, uint8 rspidlistlen, uint16 *rspid,	uint8 *rsplen,
                 void *rspdata, uint8 *buf )
{
    uint8 state, check, length;
    int i;
	uint16 cmd_id;
    
    state = 0;
    check = 0;
    do {
        if( 0 == state ) {
            if( treadn(zigbee_fd, buf, 1, ZIGBEE_TIMEOUT) != 1 )
                return -1;
            pr_mem( buf, 1 );
            if( 0xFE == buf[0] )
                state = 1;
        } else if( 1 == state ) {
            check = 0;
            // read length and cmd
            if( treadn(zigbee_fd, buf, 3, ZIGBEE_TIMEOUT) != 3 )
                return -1;
            pr_mem(buf, 3);
            for( i = 0; i < 3; ++i )
                check ^= buf[i];
            length = buf[0];
            // cmd check
            cmd_id = to_u16(buf + 1);
            if( !find_in_array(cmd_id, rspidlist, rspidlistlen) )
                state = 0;
            else {
				if( rspid ) *rspid = cmd_id;
                // read data
                if( treadn(zigbee_fd, buf, length+1, ZIGBEE_TIMEOUT) != length+1 )
                    return -1;
                pr_mem(buf, length+1 );
                // compute check
                for( i = 0; i < length; ++i )
                    check ^= buf[i];
                if( check != buf[length] )
                    return 2;
                else {
                    if( rsplen )
                        *rsplen = length;
                    if( rspdata )
                        memcpy( rspdata, buf, length );
                    state = 2;
                } // checksum correct
            } // cmd check correct
        }
    }while( state < 2 );
    
    return 0;
}

int zigbee_request( uint16 reqid, uint8 reqlen, void *reqdata, 
		uint16 rspid, uint8 *rsplen, void *rspdata,
		uint16 callbackid, uint8 *callbacklen, void *callbackdata )
{
	static uint8 buf[ZIGBEE_BUFLEN];
	int len, ret;
    uint16 cmdId;
    uint8 cmdLen;
    uint16 cmdIdList[2];
    uint16 *cmdIdListPtr = cmdIdList;
    uint8 cmdIdListLen = 0;

	len = make_zigbee_frame( reqid, reqlen, reqdata, buf );

	DBG( "Sending request: frame length=%d", len );
	pr_mem(buf, len);

	if( writen(zigbee_fd, buf, len) != len )
		return -1;
    
    if( ZIGBEE_INVALID_CMD == rspid && ZIGBEE_INVALID_CMD == callbackid )
        return 0;
    
    if( ZIGBEE_INVALID_CMD != rspid ) {
        cmdIdList[0] = rspid;
        ++cmdIdListLen;
    }
    
    if( ZIGBEE_INVALID_CMD != callbackid ) {
        cmdIdList[1] = callbackid;
        if( ++cmdIdListLen == 1 )
            ++cmdIdListPtr;
    }

AGAIN:
    ret = get_response( cmdIdListPtr, cmdIdListLen, &cmdId, &cmdLen, buf, buf );
    if( ret < 0 ) {
        DBG( "get_response error. %d", ret );
        return ret;
    }
    
    if( cmdId == rspid ) {
        if( rsplen ) *rsplen = cmdLen;
        if( rspdata )
            memcpy( rspdata, buf, cmdLen );
        if( --cmdIdListLen == 0 )
            return 0;
        else {
            ++cmdIdListPtr;
            goto AGAIN;
        }
    } else if( cmdId == callbackid ) {
        if( callbacklen ) *callbacklen = cmdLen;
        if( callbackdata )
            memcpy( callbackdata, buf, cmdLen );
        if( --cmdIdListLen == 0 )
            return 0;
        else {
            goto AGAIN;
        }
    }
    
    return 0;
}

//int zigbee_get_node_desc( uint16 addr )
//{
//    uint8 buf[5];
//    uint8 *ptr = buf;
//    uint8 len;
//    int ret;
//    
//    DBG( "zigbee_get_node_desc %04x", addr );
//    
//    // dst addr = this addr
//    memcpy( ptr, &zigbee_dev_info.shortAddr, 2 );
//    ptr += 2;
//    memcpy( ptr, &addr, 2 );
//
//    ret = zigbee_request( ZDO_NODE_DESC_REQ, 4, buf, ZDO_NODE_DESC_RSP,
//                         &len, buf, ZDO_NODE_DESC_CB, <#uint8 *callbacklen#>, <#void *callbackdata#>)
//    
//    return 0;
//}

// AF
int zigbee_send_data( uint16 dstSAddr, uint8 dstep, uint8 srcep, uint16 clusterID, uint8 seqno, uint8 opt,
                     uint8 radius, uint8 len, void *data )
{
    uint8 buf[ZIGBEE_BUFLEN];
    uint8 *ptr = buf;
    int ret;
    uint8 go_on = 1;
    uint8 callbackbuf[5];
    uint16 cmd = AF_DATA_CB;
    
    DBG( "zigbee_send_data dst = %04x", dstSAddr );
    
    memcpy( ptr, &dstSAddr, 2 );
    ptr += 2;
    *ptr++ = dstep;
    *ptr++ = srcep;
    memcpy( ptr, &clusterID, 2 );
    ptr += 2;
    *ptr++ = seqno;                     // handle
    *ptr++ = opt;
    *ptr++ = radius;
    *ptr++ = len;
    memcpy( ptr, data, len );
    ptr += len;
    
    ret = zigbee_request( AF_DATA_REQ, (uint8)(ptr-buf), buf,
                         AF_DATA_RES, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );
    
    if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}
    
    ret = buf[0];
    if( ret ) {
        DBG( "AF_DATA_RES error, %02x.", ret );
        return ret;
    }
    
    // wait for data confirm
    do {
        ret = get_response( &cmd, 1, &cmd, &len, callbackbuf, buf );
        if( ret ) {
            DBG( "wait for feedback error, %d", ret );
            return ret;
        }
        if( callbackbuf[2] == seqno ) {
            go_on = 0;
            ret = callbackbuf[0];
            if( ret ) {
                DBG( "AF_DATA_CB error, %d.", ret );
                return ret;
            }
        } // if seqno
    } while( go_on );
    
    return 0;
}

// SAPI
//!! data len check
//!! ZB_SEND_DATA_CB maybe the feedback of other sending calls, we have to check it.
//int zigbee_send_data( uint16 dstSAddr, uint16 clusterID, uint8 seqno, uint8 ack,
//                     uint8 radius, uint8 len, void *data )
//{
//    uint8 buf[ZIGBEE_BUFLEN];
//    uint8 *ptr = buf;
//    int ret;
//    uint8 go_on = 1;
//    uint8 callbackbuf[3];
//    
//    DBG( "zigbee_send_data dst = %04x", dstSAddr );
//    
//    memcpy( ptr, &dstSAddr, 2 );
//    ptr += 2;
//    memcpy( ptr, &clusterID, 2 );
//    ptr += 2;
//    *ptr++ = seqno;                     // handle
//    *ptr++ = ack;
//    *ptr++ = radius;
//    *ptr++ = len;
//    memcpy( ptr, data, len );
//    ptr += len;
//    
//    ret = zigbee_request( ZB_SEND_DATA_REQ, (uint8)(ptr-buf), buf,
//                ZB_SEND_DATA_RSP, NULL, NULL, ZIGBEE_INVALID_CMD, NULL, NULL );
//    
//    if( ret ) {
//		DBG( "zigbee_request failed with code %d.", ret );
//		return ret;
//	}
//    
//    // wait for data confirm
//    do {
//        ret = get_response( ZB_SEND_DATA_CB, &len, callbackbuf, buf );
//        if( ret ) {
//            DBG( "wait for feedback error, %d", ret );
//            return ret;
//        }
//        if( callbackbuf[0] == seqno ) {
//            go_on = 0;
//            ret = buf[1];
//            if( ret ) {
//                DBG( "Data confirm error %02x.", ret );
//                return ret;
//            }
//        } // if seqno
//    } while( go_on );
//
//    return 0;
//}

int zigbee_end_dev_bind( uint16 coordSAddr, uint8 *coordEAddr, uint8 endpoint, uint16 profileid,
        uint8 nInCluster, uint16 *pInClusters, uint8 nOutCluster, uint16 *pOutClusters )
{
    int ret;
    uint8 buf[ZIGBEE_BUFLEN], cb_buf[10];
    uint8 rsplen, cblen;
    uint8 *ptr;
    uint16 srcAddr;
    
    DBG( "zigbee_end_dev_bind, coordSAddr = %04x", coordSAddr );
    
    // too many clusters error
    if( (2 * (nInCluster + nOutCluster) + 22) > ZIGBEE_BUFLEN ) {
        DBG( "too long command.");
        return 2;
    }
    
    // Destination address is always 0x0000.
    memset( buf, 0, sizeof(buf) );
    ptr = buf + 2;
    // local coordinator
    memcpy( ptr, &coordSAddr, 2 );
    ptr += 2;
    // ieee addr of coordinator
    memcpy( ptr, coordEAddr, 8 );
    ptr += 8;
    // end point
    *ptr++ = endpoint;
    // profile id
    memcpy( ptr, &profileid, 2 );
    ptr += 2;
    // input clusters
    *ptr++ = nInCluster;
    if( nInCluster )
        memcpy( ptr, pInClusters, 2 * nInCluster );
    ptr += 2 * nInCluster;
    // output clusters
    *ptr++ = nOutCluster;
    if( nOutCluster )
        memcpy( ptr, pOutClusters, 2 * nOutCluster );
    ptr += 2 * nOutCluster;
    
    ret = zigbee_request( ZDO_END_DEVICE_BIND_REQ, (uint8)(ptr-buf),
                buf, ZDO_END_DEVICE_BIND_RSP, &rsplen, buf, ZDO_END_DEVICE_BIND_CB,
                &cblen, cb_buf );
    
	if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}
    
    // check rsp status
    pr_mem( buf, 1 );
    ret = buf[0];
    if( ret ) {
        DBG( "ZDO_END_DEVICE_BIND_RSP fail with code %d.", ret );
        return ret;
    }
    
    //!! get callback, can only get ONE
    srcAddr = to_u16(cb_buf);
    ret = cb_buf[2];
    DBG( "Binding %04x %s", srcAddr, ret ? "fail" : "success" );
    
    return 0;
}

int zigbee_start()
{
    int ret;
    uint8 buf[5];
    uint8 len;
    
    DBG( "zigbee_start" );
    
    memset( buf, 0, sizeof(buf) );
    
    ret = zigbee_request( ZDO_STARTUP_FROM_APP_REQ, 2, buf, ZDO_STARTUP_FROM_APP_RES, &len, buf,
                         ZIGBEE_INVALID_CMD, NULL, NULL );
    
	if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}
    
    return 0;
}

// SAPI
//int zigbee_start()
//{
//    int ret;
//    uint8 buf[5];
//    uint8 len;
//    
//    DBG( "zigbee_start" );
//    
//    ret = zigbee_request( ZB_START_REQ, 0, NULL, ZB_START_RSP, NULL, NULL,
//                         ZB_START_CB, &len, buf );
//    
//	if( ret ) {
//		DBG( "zigbee_request failed with code %d.", ret );
//		return ret;
//	}
//    
//    // check status
//    ret = buf[0];
//    if( ret ) {
//        DBG( "ZB_START_CB fail.");
//        return ret;
//    }
//    
//    return 0;
//}

static
int zigbee_get_device_info_kv( uint8 id, uint8 *pvalue )
{
    int ret;
    uint8 buf[20];
    uint8 len;
    
    buf[0] = id;
    
    ret = zigbee_request( ZB_GET_DEVICE_INFO_REQ, 1, buf, ZB_GET_DEVICE_INFO_RSP,
                         &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );
    
	if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}
    
    if( id != buf[0] )
        return -1;
    
    memcpy( pvalue, buf + 1, 8 );
    
    return 0;
}

void zigbee_get_device_info()
{
    uint8 buf[8];
    // state
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_STATE, buf );
    zigbee_dev_info.state = buf[0];
    // short addr
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_SHORT_ADDR, buf );
    zigbee_dev_info.shortAddr = to_u16( buf );
    // ieee addr
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_IEEE_ADDR, buf );
    memcpy( zigbee_dev_info.ieee_addr, buf, 8 );
    // pan id
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_PANID, buf );
    zigbee_dev_info.panID = to_u16( buf );
    // ext pan id
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_EXT_PANID, buf );
    memcpy( zigbee_dev_info.extPanID, buf, 8 );
    // channel
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_CHANNEL, buf );
    zigbee_dev_info.channel = buf[0];
    // parent short addr
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_PARENT_SHORT_ADDR, buf );
    zigbee_dev_info.parentSAddr = to_u16( buf );
    // parent ieee addr
    zigbee_get_device_info_kv( ZB_DEVICE_INFO_PARENT_IEEE_ADDR, buf );
    memcpy( zigbee_dev_info.parentIeeeAddr, buf, 8 );
    
    return;
}

// AF
int zigbee_app_register( uint8 endpoint, uint16 profileid, uint16 devid,
                        uint8 devvVer, uint8 nInCluster, uint16 *pInClusters,
                        uint8 nOutCluster, uint16 *pOutClusters )
{
    int ret;
    uint8 buf[ZIGBEE_BUFLEN];
    uint8 len;
    uint8 *ptr = buf;

    DBG( "zigbee_app_register" );

    // too many clusters error
    if( (2 * (nInCluster + nOutCluster) + 14) > ZIGBEE_BUFLEN ) {
        DBG( "too long command.");
        return 2;
    }

    // endpoint
    *ptr++ = endpoint;
    // profile id
    memcpy( ptr, &profileid, 2 );
    ptr += 2;
    // device id
    memcpy( ptr, &devid, 2 );
    ptr += 2;
    // device version
    *ptr++ = devvVer;
    // latency
    *ptr++ = 0;
    // input clusters
    *ptr++ = nInCluster;
    if( nInCluster )
        memcpy( ptr, pInClusters, 2 * nInCluster );
    ptr += 2 * nInCluster;
    // output clusters
    *ptr++ = nOutCluster;
    if( nOutCluster )
        memcpy( ptr, pOutClusters, 2 * nOutCluster );
    ptr += 2 * nOutCluster;

    ret = zigbee_request( AF_REGISTER_REQ, (uint8)(ptr-buf), buf,
                    AF_REGISTER_RSP, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );

	if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}

    // check status
    ret = buf[0];
    if( ret ) {
        DBG( "AF_REGISTER_RSP fail.");
        return ret;
    }

    return 0;
}

//// SAPI
//int zigbee_app_register( uint8 endpoint, uint16 profileid, uint16 devid,
//                        uint8 devvVer, uint8 nInCluster, uint16 *pInClusters,
//                        uint8 nOutCluster, uint16 *pOutClusters )
//{
//    int ret;
//    uint8 buf[ZIGBEE_BUFLEN];
//    uint8 len;
//    uint8 *ptr = buf;
//    
//    DBG( "zigbee_app_register" );
//    
//    // too many clusters error
//    if( (2 * (nInCluster + nOutCluster) + 14) > ZIGBEE_BUFLEN ) {
//        DBG( "too long command.");
//        return 2;
//    }
//    
//    // endpoint
//    *ptr++ = endpoint;
//    // profile id
//    memcpy( ptr, &profileid, 2 );
//    ptr += 2;
//    // device id
//    memcpy( ptr, &devid, 2 );
//    ptr += 2;
//    // device version
//    *ptr++ = devvVer;
//    // skip unused
//    *ptr++ = 0;
//    // input clusters
//    *ptr++ = nInCluster;
//    if( nInCluster )
//        memcpy( ptr, pInClusters, 2 * nInCluster );
//    ptr += 2 * nInCluster;
//    // output clusters
//    *ptr++ = nOutCluster;
//    if( nOutCluster )
//        memcpy( ptr, pOutClusters, 2 * nOutCluster );
//    ptr += 2 * nOutCluster;
//    
//  
//    ret = zigbee_request( ZB_APP_REGISTER_REQUEST_REQ, (uint8)(ptr-buf), buf,
//                    ZB_APP_REGISTER_REQUEST_RSP, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );
//    
//	if( ret ) {
//		DBG( "zigbee_request failed with code %d.", ret );
//		return ret;
//	}
//    
//    // check status
//    ret = buf[0];
//    if( ret ) {
//        DBG( "ZB_APP_REGISTER_REQUEST_RSP fail.");
//        return ret;
//    }
//
//    return 0;
//}

int zigbee_reset()
{
	int ret;
	uint8 buf[10];
	uint8 len;

	DBG( "zigbee_reset" );
	
	buf[0] = 1;
	ret = zigbee_request( SYS_RESET_REQ, 1, buf, ZIGBEE_INVALID_CMD, NULL, NULL,
                         SYS_RESET_CB, &len, buf );

	if( ret ) {
		DBG( "zigbee_request failed with code %d.", ret );
		return ret;
	}

	pr_mem( buf, len );

	return 0;
}

int read_nv_config( uint16 configid, uint8 *length, void *value )
{	
	int ret;
	uint8 buf[ZIGBEE_BUFLEN], len;

	memcpy( buf, &configid, 2 );
	buf[2] = 0;
	
	ret = zigbee_request( SYS_OSAL_NV_READ_REQ, 3, buf, 
			SYS_OSAL_NV_READ_RSP, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );

	if( ret ) {
		DBG( "read_nv_config error with code %d.", ret );
		return ret;
	}

//	DBG( "read_nv_config get response, length = %u", len );
//	pr_mem( buf, len );

	// read status
	ret = buf[0];
	if( ret ) {
		DBG( "read_nv_config read error with code: %02x", buf[0] );
		return ret;
	}
//#ifdef _DEBUG
//	else
//		DBG( "read_nv_config reads success!" );
//#endif

	// read length
	if( length )
		*length = buf[1];

	// read data
	if( value )
		memcpy( value, buf+2, *length );
//	DBG( "read_nv_config has read %u length data %04x:" , *length, configid );
//	pr_mem( value, *length );

	return 0;
}

int write_nv_config( uint16 configid, uint8 length, void *value )
{	
	int ret;
	uint8 buf[ZIGBEE_BUFLEN], len, *ptr;

again:	
	ptr = buf;
	memcpy( ptr, &configid, 2 );
	ptr += 2;
	*ptr++ = 0;		// offset
	*ptr++ = length;
	memcpy( ptr, value, length );

	ret = zigbee_request( SYS_OSAL_NV_WRITE_REQ, length + 4, buf, 
			SYS_OSAL_NV_WRITE_RSP, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );

	if( ret ) {
		DBG( "write_nv_config error with code %d.", ret );
		return ret;
	}

	DBG( "write_nv_config get response, length = %u", len );
	pr_mem( buf, len );

	// read status
	ret = buf[0];
	if( ret ) {
		DBG( "write_nv_config read error with code: %02x", buf[0] );
		if( NV_ITEM_UNINIT == ret ) {
			DBG( "Uninitialized item, init it." );
			init_nv_config(configid, length, value);
			goto again;
		}
		else
			return ret;
	}

	return 0;
}

static
int init_nv_config( uint16 configid, uint8 length, void *value )
{
	int ret;
	uint8 buf[ZIGBEE_BUFLEN], len, *ptr;
	
	ptr = buf;
	memcpy( ptr, &configid, 2 );
	ptr += 2;
	// item len 2 bytes
	*ptr++ = length;
	*ptr++ = 0;
	// init len 1 byte
	*ptr++ = length;
	memcpy( ptr, value, length );

	ret = zigbee_request( SYS_OSAL_NV_ITEM_INIT_REQ, length + 5, buf, 
			SYS_OSAL_NV_ITEM_INIT_RSP, &len, buf, ZIGBEE_INVALID_CMD, NULL, NULL );

	if( ret ) {
		DBG( "init_nv_config error with code %d.", ret );
		return ret;
	}

	// read status
	ret = buf[0];
	if( ret ) {
		DBG( "init_nv_config fail with the code %d.\n", ret );
		return ret;
	}

	return 0;
}

static
int check_init_nv(uint16 id, void *expected, uint8 len, const char *name)
{
    uint8 buf[256];
    int ret;
    uint8 datalen;
    uint8 changed = 0;
    
    ret = read_nv_config( id, &datalen, buf );
    if( ret ) {
        DBG( "read %s error: %d\n", name, ret );
        return -1;
    }
    DBG( "%s = ", name );
    pr_mem( buf, datalen );
    if( len != datalen || memcmp(buf, expected, len) ) {
        changed = 1;
        DBG( "The value of %s is not as expected, reset it.", name );
        memcpy(buf, expected, len);
        ret = write_nv_config( id, len, buf );
        if( ret ) {
            DBG( "write_nv_config failed: %d\n", ret );
            return -1;
        }
    } // if
    
    return changed;
}


int zigbee_init( uint8 type, uint8 clearOpt )
{
    uint32 value;
    uint8 buf[20];
    int ret;
    uint8 f_reset = 0;
    
    // clear startup opt if type changes
    ret = read_nv_config( ZCD_NV_LOGICAL_TYPE, &buf[1], buf );
    if( ret ) {
        DBG( "zigbee_init read config error." );
        return ret;
    } else if( buf[0] != type ) {
        clearOpt = 0x03;
	}
    
    // clear startup opt
    buf[0] = clearOpt;
    check_init_nv(ZCD_NV_STARTUP_OPTION, buf, 1, "ZCD_NV_STARTUP_OPTION");
    zigbee_reset();
    
    // check others
    memcpy( buf, &type, sizeof(type) );
    ret = check_init_nv(ZCD_NV_LOGICAL_TYPE, buf, 1, "ZCD_NV_LOGICAL_TYPE");
    if( ret == 1 ) f_reset = 1;
    value = 0x0000FFFF;
    memcpy( buf, &value, sizeof(value) );
    ret = check_init_nv(ZCD_NV_PANID, buf, 2, "ZCD_NV_PANID");
    if( ret == 1 ) f_reset = 1;
    value = 0x00100000;
    memcpy( buf, &value, sizeof(value) );
    ret = check_init_nv(ZCD_NV_CHANLIST, buf, 4, "ZCD_NV_CHANLIST");
    if( ret == 1 ) f_reset = 1;
    value = 1;
    memcpy( buf, &value, sizeof(value) );
    ret = check_init_nv(ZCD_NV_ZDO_DIRECT_CB, buf, 1, "ZCD_NV_ZDO_DIRECT_CB");
    if( ret == 1 ) f_reset = 1;
    
    if( f_reset ) {
        DBG( "check init value failed, now reset the device!" );
        ret = zigbee_reset();
    }
    
    return ret;
}

// for testing
void zigbee_print_device_info()
{
    printf( "State:\t\t\t%u\n", zigbee_dev_info.state );
    printf( "Short Address:\t\t\t%04x\n", zigbee_dev_info.shortAddr );
    printf( "IEEE Address:\t\t\t" );
    pr_mem( zigbee_dev_info.ieee_addr, 8 );
    printf( "PAN ID:\t\t\t%04x\n", zigbee_dev_info.panID );
    printf( "Extended PAN ID:\t\t\t" );
    pr_mem( zigbee_dev_info.extPanID, 8 );
    printf( "Channel:\t\t\t%u\n", zigbee_dev_info.channel );
    printf( "Parent Short Address:\t\t\t%04x\n", zigbee_dev_info.parentSAddr );
    printf( "Parent IEEE Address:\t\t\t" );
    pr_mem( zigbee_dev_info.parentIeeeAddr, 8 );
}

int zigbee_print_binding_table( uint16 addr )
{
    uint8 buf[ZIGBEE_BUFLEN];
    uint8 *ptr = buf;
    uint8 count;
    int ret;
    
    if( 0 == addr )
        addr = zigbee_dev_info.shortAddr;

    memcpy( ptr, &addr, 2 );
    ptr += 2;
    // start index
    *ptr++ = 0;
    
    ret = zigbee_request( ZDO_MGMT_BIND_REQ, 3, buf, ZIGBEE_INVALID_CMD, NULL, NULL, ZDO_MGMT_BIND_CB, &count, buf );
    
    if( ret ) {
        printf( "get binding table fail, %d\n", ret );
        return ret;
    }
    
    ptr = buf + 2;
    if( *ptr ) {
        printf( "Get binding table error. %d\n", ret );
        return ret;
    }
    
    ptr += 3;
    count = *ptr++;
    printf( "%u entries in the binding table on this device.\n", count );
    
    while( count-- ) {
        printf( "SrcAddr:\t\t\t" );
        pr_mem( ptr, 8 );
        ptr += 8;
        printf( "SrcEndpoint:\t\t\t%02x\n", *ptr++ );
        printf( "ClusterID:\t\t\t%04x\n", to_u16(ptr) );
        ptr += 3; // skip dst addr mode
        printf( "DstAddr\t\t\t" );
        pr_mem( ptr, 8 );
        ptr += 8;
        printf( "DstEndpoint:\t\t\t%02x\n", *ptr++ );
    }
    
    return 0;
}









