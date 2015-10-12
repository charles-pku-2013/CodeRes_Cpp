#ifndef _ZIGBEE_COMMON_H
#define _ZIGBEE_COMMON_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdarg>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <algorithm>
#include <functional>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include "zcl_dataType.h"

// types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define to_u16(ptr) (*((uint16*)(ptr)))
#define to_u32(ptr) (*((uint32*)(ptr)))

#define ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

#define BYTESWAP(ptr, type) do {										\
                        uint8 *p1 = (uint8*)(ptr);				\
                        uint8 *p2 = p1 + (sizeof(type)-1);		\
                        do {									\
                            SWAP( *p1, *p2 );					\
                        } while( ++p1 < --p2 );					\
                    } while(0)

#define print_mem( ptr, len ) do { 			\
			int i;							\
			for( i = 0; i < (len); ++i )		\
				printf( "%02x ", *((uint8*)(ptr) + i) );		\
			putchar('\n');					\
		}while(0)


#define read_all( fd ) do { 				\
			uint8 data;						\
			ssize_t n;					\
			while( (n = read(fd, &data, 1)) == 1 )	\
				printf( "%02x ", data );	\
			if( n < 0 )						\
				perror( "read error!" );	\
			printf( "n = %d\n" );			\
		}while(0)

#define read_all_ascii( fd ) do { 				\
						char data; 					\
						ssize_t n;					\
						while( (n = read(fd, &data, 1)) == 1 )	\
							if( isascii(data) )					\
								printf( "%c", data );	\
							else						\
								printf( "%02x ", (uint8)data );		\
						if( n < 0 ) 					\
							perror( "read error!" );	\
						printf( "n = %d\n" );			\
					}while(0)


#define read_all_lines( fd ) do { 				\
						uint8 buf[256]; 					\
						ssize_t n;					\
						while( (n = read(fd, buf, 256)) > 0 )	\
							print_mem(buf, n );				\
						if( n < 0 ) 					\
							perror( "read error!" );	\
						printf( "n = %d\n" );			\
					}while(0)


#define WAIT( str ) do {                    \
            printf( "%s\n", (str) );          \
            getchar();                      \
        } while(0)

// DEBUG
#ifdef _DEBUG
	#define DBG( fmt, ... ) fprintf( stdout, fmt"\n", ##__VA_ARGS__ )
	#define DBG2( fmt, ... ) fprintf( stderr, fmt, ##__VA_ARGS__ )
	#define pr_mem( ptr, len ) print_mem( ptr, len )
	#define pr_mem2( ptr, len ) do { 			\
				int i;							\
				for( i = 0; i < (len); ++i )		\
					fprintf( stderr, "%02x ", *((uint8*)(ptr) + i) );		\
			}while(0)
#define pr_mem2_ascii( ptr, len ) do { 			\
				int i;							\
				for( i = 0; i < (len); ++i )		\
					if( isprint(*((char*)(ptr) + i)) )		\
						fprintf( stderr, "%c", *((char*)(ptr) + i) );		\
					else													\
						fprintf( stderr, "%02x ", *((uint8*)(ptr) + i) );		\
			}while(0)			
#else
	#define DBG
	#define DBG2
	#define pr_mem
	#define pr_mem2
#endif

#define HA_PROFILEID		0x0104
#define ZIGBEE_TIMEOUT 10
#define ZIGBEE_BUFLEN 512
#define MAX_AT_CMD_LEN	15

// profile ids
// ha
#define ZCL_HA_PROFILE_ID             						0x0104
// devices
#define ZCL_HA_DEVICEID_ON_OFF_SWITCH  						0x0000
// cluster ids
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_UBEC_POWERMETER						0x04FD

/* enums */
typedef enum {
	NOTPRESENT,
	GROUP,
	SHORT,
	EXTENDED
} AddrType_t;

typedef struct {
	AddrType_t 	type;
	union {
		uint16	shortAddr;
		uint8	ieeeAddr[8];
	}addrData;
} Address_t;

#ifdef _DEBUG
extern void printAddr( const Address_t *addr );
#endif

extern bool operator== ( const Address_t &lhs, const Address_t &rhs );
extern bool operator!= ( const Address_t &lhs, const Address_t &rhs );


typedef enum {
	API_ATCMD,
	API_P2P,
	API_GRP,
	API_REMOTEAT,
	API_STAR,
	API_RAW
} CmdType_t;

// ZCL frame specific
typedef enum {
    ZCL_FRAME_GENERAL,
    ZCL_FRAME_CLUSTER_SPEC,
    ZCL_FRAME_MANU_SPEC = 4,
    ZCL_FRAME_FROME_SERVER = 8,
    ZCL_FRAME_NO_RSP = 16
} ZclFrameControl_t;

//typedef struct {
//    uint8			frameCtrl;
//    uint16          manufacturerCode;
//    uint8           seqNO;
//    uint8           cmd;
//    void*           payload;
//	uint16			payloadLen;		// not part of zcl frame
//} ZclFrame_t;

typedef std::basic_string<uint8> PayloadType;

class ZclFrame {
public:
    typedef enum {
        ZCL_FRAME_GENERAL,
        ZCL_FRAME_CLUSTER_SPEC,
        ZCL_FRAME_MANU_SPEC = 4,
        ZCL_FRAME_FROM_SERVER = 8,
        ZCL_FRAME_NO_RSP = 16
    } ZclCtrlType;
public:
    ZclFrame() : _frameCtrl(0) {}

	void setCtrl( uint8 ctrlByte )
	{ _frameCtrl = ctrlByte; }
    void setCtrl( ZclCtrlType ctrlField )
    { _frameCtrl |= ctrlField; }  
    void unsetCtrl( ZclCtrlType ctrlField )
    { _frameCtrl &= ~ctrlField; }
	uint8 frameCtrl() const
	{ return _frameCtrl; }
    
    void setSeqNO( uint8 seqNO )
    { _seqNO = seqNO; }
    uint8 seqNO() const { return _seqNO; }
    
    void setCmd( uint8 cmdId )
    { _cmdId = cmdId; }
    uint8 cmd() const { return _cmdId; }

    void setPayload( const uint8 *start, const uint8 *end )
    { _payload.assign( start, end ); }
    void setPayload( const void *vptr, PayloadType::size_type len )
    { 
    	const uint8 *ptr = (const uint8*)vptr;
    	_payload.assign( ptr, len ); 
	}
    void appendPayload( const uint8 *start, const uint8 *end )
    { _payload.append( start, end ); }
    void appendPayload( const void *vptr, PayloadType::size_type len )
    { 
    	const uint8 *ptr = (const uint8*)vptr;
    	_payload.append( ptr, len ); 
	}	
    const uint8 *payload() const
    { return (payloadLen() ? _payload.c_str() : NULL); }
    uint16 payloadLen() const
    { return (uint16)_payload.size(); }
    
    //!! & has lower predencedence than ==
    bool isClusterSpec() const
    { return ((_frameCtrl & ZCL_FRAME_CLUSTER_SPEC) == ZCL_FRAME_CLUSTER_SPEC); }
    
    bool isManuSpec() const
    { return ((_frameCtrl & ZCL_FRAME_MANU_SPEC) == ZCL_FRAME_MANU_SPEC); }
    
    bool isFromServer() const
    { return ((_frameCtrl & ZCL_FRAME_FROM_SERVER) == ZCL_FRAME_FROM_SERVER); }
    
    bool responseDisabled() const
    { return ((_frameCtrl & ZCL_FRAME_NO_RSP) == ZCL_FRAME_NO_RSP); }
    
    void setManuCode( uint16 manuCode )
    { _manufacturerCode = manuCode; }
    uint16 manuCode() const
    { return _manufacturerCode; }
protected:
    uint8			_frameCtrl;
    uint16          _manufacturerCode;
    uint8           _seqNO;
    uint8           _cmdId;
    PayloadType  	_payload;
#ifdef _DEBUG
public:
    void print()
    {
        printf( "ZCL Frame seqNO:\t\t\t%02x\n", seqNO() );
		printf( "FrameCtrl:\t\t\t%02x\n", _frameCtrl );
        printf( "FrameCtrl:\t\t\t%s %s %s %s\n", isClusterSpec() ? "ClusterSpec" : "",
               isManuSpec() ? "ManuSpec" : "", isFromServer() ? "FromServer" : "",
               responseDisabled() ? "responseDisabled" : "" );
        if( isManuSpec() )
            printf( "ManuCode:\t\t\t%04x\n", manuCode() );
        printf( "CmdID:\t\t\t%02x\n", cmd() );
        printf( "payload:\t\t\t" );
        pr_mem( payload(), payloadLen() );
    }
#endif
};


typedef enum {
	COORD, ROUTER, ENDDEV
} ZigbeeDevType_t;

// find whether two obj like callback or device is equal, for find_if()
typedef enum { 
	CMPBYADDR = 0x01, CMPBYPROFILE = 0x01 << 1, CMPBYCLUSTER = 0x01 << 2,
	CMPBYSRCEP = 0x01 << 3, CMPBYDSTEP = 0x01 << 4, CMPBYSEQNO = 0x01 << 5,
	CMPBYALL = (0x01 << 6) - 1	//!! should not use ~0
} ZigbeeCmpType_t;

class ZigbeeDevice {
public:
	//!! for ubec, only support HA_PROFILEID, and using 0 endpoint
	ZigbeeDevice() : _profileId( HA_PROFILEID ), _endpoint(0), _seqNO(0) {}

	void setDevID( uint16 id )
	{ _devID = id; }
	virtual uint16 devID() const
	{ return _devID; }	
	
	uint8 endpoint() const
	{ return _endpoint; }
	void setEndpoint( uint8 ep )
	{ _endpoint = ep; }

	uint8 seqNO()
	{ return ++_seqNO; }
	
	uint16 profileId() const
	{ return _profileId; }
	void setProfileId( uint16 profileId )
	{ _profileId = profileId; }
	
	uint8 channel() const
	{ return _channel; }
	void setChannel(uint8 channel)
	{ _channel = channel; }

	uint8 security() const
	{ return _security; }
	void setSecurity(uint8 security)
	{ _security = security; }

	ZigbeeDevType_t role() const
	{ return _role; }
	void setRole(ZigbeeDevType_t role)
	{ _role = role; }
	void setRole(int role)
	{ 
		if( role > ENDDEV )
			role = ENDDEV;
		_role = (ZigbeeDevType_t)role; 
	}	

	uint16 panId() const
	{ return _panId; }
	void setPanId(uint16 panid)
	{ _panId = panid; }

	uint16 addr() const
	{ return _addr; }
	void setAddr(uint16 addr)
	{ _addr = addr; }

	uint16 p2pAddr() const
	{ return _p2pAddr; }
	void setP2pAddr(uint16 p2pAddr)
	{ _p2pAddr = p2pAddr; }

	uint8 *macAddr()
	{ return _macAddr; }
	void setMacAddr(uint8 *macAddr)
	{ memcpy( _macAddr, macAddr, 8 ); }
	void setMacAddr( const char *macStr )
	{
		int			j;
		char		tmp[3];
		uint8		data;
		uint8 		*pDst = _macAddr;

		tmp[2] = 0;
		for( j = 0; j < 8; ++j, macStr += 2 ) {
			strncpy( tmp, macStr, 2 );
			data = (uint8)strtol(tmp, NULL, 16);
			*pDst++ = data;			
		} // for
	}
	
#ifdef _DEBUG	
	void print()
	{
		char buf[20];
	
		printf( "This device info:\n" );
		printf( "Channel:\t\t\t%u\n", _channel );
		printf( "PAN ID:\t\t\t%04x\n", _panId );
		switch( _role ) {
		case COORD:
			strcpy( buf, "Coord" );
			break;
		case ROUTER:
			strcpy( buf, "Router" );
			break;
		case ENDDEV:
			strcpy( buf, "End Device" );
			break;		
		} // switch 
		printf( "Device Type:\t\t\t%s\n", buf );
		printf( "Address:\t\t\t%04x\n", _addr );
		printf( "MAC Addr:\t\t\t" );
		print_mem( _macAddr, 8 );
		printf( "P2P Addr:\t\t\t%04x\n", _p2pAddr );
		printf( "Security:\t\t\t%s\n", _security ? "ON" : "OFF" );
	}
#endif
protected:
	uint16						_devID;
	uint16						_profileId;
	uint8						_endpoint;
	uint8						_channel;
	uint8						_security;
	uint8						_seqNO;		// for making zcl frame
	ZigbeeDevType_t				_role;
	uint16						_panId;
	uint16						_addr;
	uint16						_p2pAddr;
	uint8						_macAddr[8];
};

typedef ZigbeeDevice* ZigbeeDevicePtr;

// global vars declaration
extern int zigbee_fd;
extern ZigbeeDevice thisDev;

extern uint8 make_address( Address_t *addr, AddrType_t addrType, void *data );
extern int zigbee_zcl_cmd( Address_t *dstAddress, uint8 srcep, uint8 dstep, 
		uint16 profileID, uint16 clusterID, const ZclFrame *pZclFrame, 
		int timeout = 0, void **result = 0, uint16 *resultLen = 0 );


typedef int (*ZclCmdFunc)( ZigbeeDevice &dev, int timeout );

struct ZclAttr {
	ZclAttr() {}
	ZclAttr( uint16 id, const char *_name, uint8 type, bool avail = true )
		: attrID(id), name(_name), dataType(type), available(avail) {}

    void setData( const uint8 *start, const uint8 *end )
    { _data.assign( start, end ); }
    void setData( const void *vptr, PayloadType::size_type len )
    { 
    	const uint8 *ptr = (const uint8*)vptr;
    	_data.assign( ptr, len ); 
	}
    void appendData( const uint8 *start, const uint8 *end )
    { _data.append( start, end ); }
    void appendData( const void *vptr, PayloadType::size_type len )
    { 
    	const uint8 *ptr = (const uint8*)vptr;
    	_data.append( ptr, len ); 
	}	
    const uint8 *data() const
    { return (dataLen() ? _data.c_str() : NULL); }
    uint16 dataLen() const
    { return (uint16)_data.size(); }

	
	uint16			attrID;
	const char		*name;
	uint8			dataType;
	bool			available;
	PayloadType		_data;
};
typedef std::vector<ZclAttr> ZclAttrList;

struct ZclCmd {
//	ZclCmd() {}
//	ZclCmd( uint8 id, const char *_name, ZclCmdFunc _func, bool avail = true )
//		: cmdID(id), name(_name), available(avail) {}
	uint8				cmdID;
	const char			*name;
	ZclCmdFunc			func;	
};
typedef std::vector<ZclCmd> ZclCmdList;

class ZclBase {
//	static const uint16			_clusterID = 0x0;
public:
	typedef enum { CMD_READ_ATTR = 0x0, CMD_DISCOVER_ATTR = 0x0C } GenCmd;
public:
	virtual uint16 clusterID() const { return ZCL_CLUSTER_ID_GEN_BASIC; }
public:	
	static int readAttr( ZigbeeDevice &dev, uint16 clusterId, uint16 attrID, 
					uint8 &rspDataType, PayloadType &resultData )
	{
		int 			ret = 0;
		uint16			addrData;
		Address_t		addr;
		void			*resultPtr;	
		ZclFrame		*pResultFrame = 0;
		const uint8 	*pRspData;
		uint16			rspAttrID;	
		uint8			rspStatus;
		ZclFrame 		reqFrame;

		DBG( "ZclBase::readAttr() clusterID = %04x, attrID = %04x", clusterId, attrID );

		addrData = dev.addr();
		make_address( &addr, SHORT, &addrData );
		
		reqFrame.setSeqNO( dev.seqNO() );
		reqFrame.setCmd( (uint8)CMD_READ_ATTR );

#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &attrID, uint16 );
#endif
		reqFrame.setPayload( &attrID, 2 );

		ret = zigbee_zcl_cmd( &addr, thisDev.endpoint(), dev.endpoint(), dev.profileId(), clusterId, &reqFrame, 
				ZIGBEE_TIMEOUT, &resultPtr );

		if( ret ) {
			DBG( "zigbee_zcl_cmd fail, %d", ret );
			goto RET;
		}
		if( !resultPtr ) {
			DBG( "callback returned null data." );
			ret = 1;
			goto RET;
		}
		
		DBG( "readAttrTest got callback data." );
		pResultFrame = (ZclFrame*)resultPtr;
		pResultFrame->print();

		pRspData = pResultFrame->payload();
		rspAttrID = to_u16( pRspData );
		pRspData += 2;

		if( rspAttrID != attrID ) {
			DBG( "Parse callback error." );
			ret = 1;
			goto RET;
		}

		rspStatus = *pRspData++;
		if( rspStatus ) {
			DBG( "Callback returned error." );
			ret = rspStatus;
			goto RET;
		}

		rspDataType = *pRspData++;
		resultData.assign( pRspData, pResultFrame->payload() + pResultFrame->payloadLen() );
		
RET:
		delete pResultFrame;
		return ret;
	}

//	int discoverAttr( ZigbeeDevice &dev, uint16 clusterId, uint16 startAttrID, 
//					uint8 &rspDataType,  )
//	{
//		int 			ret = 0;
//		uint16			addrData;
//		Address_t		addr;
//		void			*resultPtr; 
//		ZclFrame		*pResultFrame = 0;
//		const uint8 	*pRspData;
//		uint16			rspAttrID;	
//		uint8			rspStatus;	

//		DBG( "ZclBase::discoverAttr() clusterID = %04x, startID = %04x", clusterId, startAttrID );

//		addrData = dev.addr();
//		make_address( &addr, SHORT, &addrData );
//		
//		ZclFrame reqFrame;
//		reqFrame.setSeqNO( dev.seqNO() );
//		reqFrame.setCmd( (uint8)CMD_DISCOVER_ATTR );

//#ifdef HOST_ZCL_ENDIAN_DIFF
//		BYTESWAP( &startAttrID, uint16 );
//#endif

//	}

	// for those req zcl frames only containing cmd ID
	// and rsp zcl frame only containing cmdid and status
	static int issueZclCmd( ZigbeeDevice &dev, uint16 clusterID, uint8 cmdID, int timeout )
	{
		int 			ret = 0;		
		uint16			addrData;
		Address_t		addr;
		void			*resultPtr; 
		ZclFrame		*pResultFrame = 0;		
		ZclFrame		reqFrame;
		const uint8 	*rspData;
		uint8			rspCmd;
		uint8			rspStatus;


		DBG( "ZclBasic:issueZclCmd() %04x, %02x", clusterID, cmdID );	

		reqFrame.setCtrl( ZCL_FRAME_CLUSTER_SPEC );
		reqFrame.setSeqNO( dev.seqNO() );
		reqFrame.setCmd( cmdID );
		
		addrData = dev.addr();
		make_address( &addr, SHORT, &addrData );
		
		ret = zigbee_zcl_cmd( &addr, thisDev.endpoint(), dev.endpoint(), dev.profileId(), clusterID, 
				&reqFrame, timeout, &resultPtr );
		if( ret ) {
			DBG( "zigbee_zcl_cmd fail, %d", ret );
			goto RET;
		}

		if( !timeout )
			return 0;
		
		if( !resultPtr ) {
			DBG( "callback returned null data." );
			ret = 1;
			goto RET;
		}
		
		DBG( "got callback data:" );
		pResultFrame = (ZclFrame*)resultPtr;
#ifdef _DEBUG
		pResultFrame->print();
#endif
		if( pResultFrame->payloadLen() < 2 ) {
			ret = 1;
			goto RET;
		}

		rspData = pResultFrame->payload();
		rspCmd = *rspData++;
		rspStatus = *rspData;
		if( rspCmd != cmdID ) {
			ret = 1;
			goto RET;
		}

		ret = rspStatus;
RET:	
		delete pResultFrame;
		DBG( "ZclBase::issueZclCmd() returned %d", ret );
		return ret;
	}
};

class ZclGenOnOff : public ZclBase {
	static const uint16			_clusterID = ZCL_CLUSTER_ID_GEN_ON_OFF;
	static const int			N_ATTR = 1;
	static const int			N_CMD = 3;
	static const ZclCmd			cmds[N_CMD]; 
public:
	enum { ON_OFF }				attrList;
	enum { OFF, ON, TOGGLE }	cmdList;
public:
	ZclGenOnOff()
	{
		attrs[ON_OFF] = ZclAttr( (uint16)ON_OFF, "OnOff", ZCL_DATATYPE_BOOLEAN );

//		cmds[OFF] = ZclCmd( (uint8)OFF, "Off", off );
//		cmds[ON] = ZclCmd( (uint8)ON, "On", on );
//		cmds[TOGGLE] = ZclCmd( (uint8)TOGGLE, "Toggle", toggle );
	}
	
	static int on( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, _clusterID, (uint8)ON, timeout );
	}
	
	static int off( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, _clusterID, (uint8)OFF, timeout );
	}

	static int toggle( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, _clusterID, (uint8)TOGGLE, timeout );
	}	
	virtual uint16 clusterID() const { return _clusterID; }	
protected:
	ZclAttr			attrs[N_ATTR];
//	ZclCmd			cmds[N_CMD];
};

class ZclUbecPowerMeter : public ZclBase {
	static const uint16			_clusterID = ZCL_CLUSTER_ID_UBEC_POWERMETER;
public:
	enum { VOLTAGE = 0x0, CURRENT = 0x1000, POWER = 0x2000, ENERGY = 0x3000, 
			POWERFACTOR = 0x4000 } attrList;
public:
	ZclUbecPowerMeter()
	{
		attrs.push_back( ZclAttr((uint16)VOLTAGE, "Voltage", ZCL_DATATYPE_DATA16) );
		attrs.push_back( ZclAttr((uint16)CURRENT, "Current", ZCL_DATATYPE_DATA16) );
		attrs.push_back( ZclAttr((uint16)POWER, "Power", ZCL_DATATYPE_DATA16) );
		attrs.push_back( ZclAttr((uint16)ENERGY, "Energy", ZCL_DATATYPE_DATA16) );
		attrs.push_back( ZclAttr((uint16)POWERFACTOR, "PowerFactor", ZCL_DATATYPE_DATA16) );
	}
	
	virtual uint16 clusterID() const { return _clusterID; }
protected:
	ZclAttrList			attrs;
};

class UbecPlug : public ZigbeeDevice {
public:
	int toggle( int timeout = ZIGBEE_TIMEOUT )
	{
		return ZclGenOnOff::toggle( *this, timeout );
	}
protected:
	
};


/* Data Structures */
class thread_exception : public std::exception {
public:
	thread_exception( const char *_msg ) throw() : msg(_msg) {}
	
	virtual const char* what() const throw()
	{ return msg; }
protected:
	const char *msg;
};

template < typename T >
class ThreadSafeList : public std::list<T> {
	typedef typename std::list<T> BaseType;
	// typedefs in STL class cannot be inherited
public:
	typedef typename BaseType::iterator Iterator;
	typedef typename BaseType::value_type ValueType;
public:
	ThreadSafeList() throw()
	{
		int err;
		err = pthread_mutex_init( &lock, NULL );
		if( err )
			throw thread_exception( "ThreadSafeList constructor init lock error" );
	}
	
	~ThreadSafeList()
	{
		pthread_mutex_destroy( &lock );
	}

	int add( const ValueType &value )
	{
		int ret = 0;
		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto RET;
		this->push_back( value );
RET:
		pthread_mutex_unlock( &lock );
//#ifdef _DEBUG
//		DBG( "ThreadSafeList::add() added element:" );
//		value->printThisDevice();
//		DBG( "list size is %u", this->size() );
//#endif		
		return ret;
	}
	
	//!! can only be used for pointers data type
	template < typename Predicate >
	ValueType find_remove( const Predicate &pred )
	{
		int				err;
		Iterator		iter;
		ValueType		retval = (ValueType)0;
		
		err = pthread_mutex_lock( &lock );
		if( err )
			goto END;
		
		iter = find_if( this->begin(), this->end(), pred );
		if( this->end() == iter )
			goto END;
		else {
			retval = *iter;
//#ifdef _DEBUG
//			DBG( "ThreadSafeList::find_remove() remove elem:" );
//			retval->print();
//#endif			
			this->erase( iter );
		}
END:		
		pthread_mutex_unlock( &lock );
//		DBG( "list size is %u", this->size() );		
		return retval;
	}

	int setLock()
	{
		int ret;
		ret = pthread_mutex_lock( &lock );
		errno = ret;
		return ret;
	}

	int releaseLock()
	{ return pthread_mutex_unlock( &lock );	}

protected:
	pthread_mutex_t lock;
};


class Callback;
typedef int (*CallbackProcessorFunc)(void*, Callback*);
class Callback {
public:
	// for status
	typedef enum { FAIL = -1, SUCCESS = 0, WAITTING = 1 } StatusType;
	// free the memory of cb data, 2nd arg means whether array or not.
	typedef void (*CbDataFreeType)(void*, bool);
public:
	// waitFlag means whether we need the response of this cmd.
	Callback( CallbackProcessorFunc _processor, uint8 _waitFlag = 0 ) 
		throw() : status(WAITTING), retval(0), processor(_processor), waitFlag(_waitFlag), 
		cbData(0), cbDataLen(0), cbDataFreeFunPtr(0)
	{
		int err;

		if( waitFlag ) {		
			err = pthread_mutex_init( &lock, NULL );
			if( err )
				throw thread_exception( "ATcommand constructor init lock error" );
			err = pthread_cond_init( &cond, NULL );
			if( err ) {
				pthread_mutex_destroy( &lock );
				throw thread_exception( "ATcommand constructor init cond var error" );
			}		
		} // if timeout
	}

	virtual ~Callback()
	{
		if( cbData && cbDataFreeFunPtr )
			cbDataFreeFunPtr( cbData, cbDataLen > 1 );
		
		if( waitFlag ) {
			pthread_mutex_destroy( &lock );
			pthread_cond_destroy( &cond );
		}
	}

	// call by the func that issues the command
	// timeout in seconds, < 0 wait until finish, = 0 return immediately with success
	// if fail on timeout, errno need to be checked
	int wait( int timeout )
	{
		int ret = 0;
		struct timeval	tv; 
		struct timespec ts;

		if( !waitFlag || !timeout )
			return 1;

		if( timeout > 0 ) {
			ret = gettimeofday( &tv, NULL );
			if( ret )
				goto FAIL;
			ts.tv_sec  = tv.tv_sec + timeout;
			ts.tv_nsec = tv.tv_usec * 1000;
		}

		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto FAIL;

		while( WAITTING == status ) {
			if( timeout > 0 ) {
				ret = pthread_cond_timedwait( &cond, &lock, &ts );
				if( ret )
					goto FAIL;
			} else {
				ret = pthread_cond_wait( &cond, &lock );
				if( ret )
					goto FAIL;
			}			
		} // while

		status = WAITTING;

		pthread_mutex_unlock( &lock );
		return retval;
FAIL:
		pthread_mutex_unlock( &lock );
		errno = ret;
		DBG( "Callback::wait() fail, %d", ret );
		return -1;
	}

	// call by the working thread, and wake up any waiters
	//!! before call it, we must check whether it exist, for it may be deleted for timeout
	//!! on timeout or abnormal return of wait, we should call this func on main thread
	// to prevent deleting-finish race condition
	void finish( StatusType _status, int _retval )
	{	
		if( waitFlag )
			pthread_mutex_lock( &lock );
		
		status = _status;
		retval = _retval;

END:
		if( waitFlag ) {
			pthread_mutex_unlock( &lock );
			pthread_cond_signal( &cond );
		}
	}
	const CallbackProcessorFunc routinePtr() const
	{ return processor; }

	int retValue() const
	{ return retval; }

	uint8 getWaitFlag() const
	{ return waitFlag; }

	// cbDataLen, when type is void, means the num of bytes, otherwise, the num of elements
	int setCbData( void *ptr, CbDataFreeType freer, uint16 len = 1 )
	{		
		if( cbData && cbDataFreeFunPtr )
			cbDataFreeFunPtr( cbData, cbDataLen > 1 );		
		cbData = ptr;
		cbDataLen = len;
		cbDataFreeFunPtr = freer;
		return 0;
	}

	//!! onece this func called by the user, the Callback obj never own the cbData, it's now the
	// user's duty to free the cbData memory.
	uint16 getCbData( void *&pData )
	{
		uint16 retval = cbDataLen;
		cbDataLen = 0;
#ifdef _DEBUG
		if( !cbData )
			DBG( "Callback::getCbData() WARNING NULL cbData" );
#endif
		pData = cbData;
		cbData = 0;
		return retval;
	}
	
protected:
	volatile StatusType		status;
	int						retval;
	// processor func call by the working thread
	// it must change the status member
	CallbackProcessorFunc 		processor;
	// for notify through cond var
	uint8					waitFlag;
	pthread_mutex_t			lock;
	pthread_cond_t			cond;
	
	// for callback data
	void*					cbData;
	uint16					cbDataLen;
	CbDataFreeType			cbDataFreeFunPtr;
#ifdef _DEBUG
public:
	virtual void print()
	{
		printf( "Callback info:\n" );		
	}
#endif
};

typedef Callback* CallbackPtr;
typedef std::list<CallbackPtr> CallbackList;

// GeneralCallback including zcl and zdp that containing profile id, cluster id and endpoints
//!! who takes the responsibility of deleting, if waitFlag is set, the caller will delete it, either
// on success return or fail like timeout, otherwise, the processor func will delete it after calling its "finish"
class GeneralCallback : public Callback {
	friend struct GeneralCallbackCmp;
public:
	GeneralCallback( const Address_t *_addr, uint16 _profileID,
		uint16 _clusterID, uint8 _srcep, uint8 _dstep, uint16 _seqNO,
		CallbackProcessorFunc _processor, uint8 _waitFlag = 0 ) 
		throw() : Callback(_processor, _waitFlag), profileID(_profileID), 
		clusterID(_clusterID), srcep(_srcep), dstep(_dstep), seqNO(_seqNO)
	{
		if( _addr )
			addr = *_addr;
	}
	
protected:
	Address_t			addr;
	uint16				seqNO;
	uint16				profileID;
	uint16				clusterID;
	uint8				srcep;
	uint8				dstep;

#ifdef _DEBUG
public:
	virtual void print()
	{
		printf( "GeneralCallback info:\n" );
		printf( "Address: " );
		printAddr( &addr );
		printf( "seqNO: %d\n", seqNO );
		printf( "profileID: %04x\n", profileID );
		printf( "clusterID: %04x\n", clusterID );
		printf( "srcep: %02x\n", srcep );
		printf( "dstep: %02x\n", dstep );
	}
#endif

}; 

typedef GeneralCallback* GeneralCallbackPtr;
typedef std::list<GeneralCallbackPtr> GeneralCallbackList;
typedef ThreadSafeList<GeneralCallbackPtr> GeneralCallbackSafeList;


// for finding in list find_if()
struct GeneralCallbackCmp {
	GeneralCallbackCmp( ZigbeeCmpType_t _method = CMPBYALL, 
			Address_t *_addr = 0, uint16 _profileID = 0, 
			uint16 _clusterID = 0, uint8 _srcep = 0, uint8 _dstep = 0, 
			uint16 _seqNO = 0 ) 
			: profileID(_profileID), clusterID(_clusterID), srcep(_srcep), 
			dstep(_dstep), seqNO(_seqNO), method(_method)
	{
		if( _addr )
			addr = *_addr;
	}

	bool operator() ( const GeneralCallbackPtr ptr ) const
	{
		if( (method & CMPBYPROFILE) && (profileID != ptr->profileID) )
			return false;
		if( (method & CMPBYCLUSTER) && (clusterID != ptr->clusterID) )
			return false;		
		if( (method & CMPBYADDR) && (addr != ptr->addr) )
			return false;
		if( (method & CMPBYSRCEP) && (srcep != ptr->srcep) )
			return false;
		if( (method & CMPBYDSTEP) && (dstep != ptr->dstep) )
			return false;
		if( (method & CMPBYSEQNO) && (seqNO != ptr->seqNO) )
			return false;
		
		return true;
	}
	
	Address_t			addr;
	uint16				seqNO;
	uint16				profileID;
	uint16				clusterID;
	uint8				srcep;
	uint8				dstep;

	ZigbeeCmpType_t		method;
};

class AtCmdCallback : public Callback {
public:
	AtCmdCallback( const char *_rspStr, CallbackProcessorFunc _processor, uint8 _waitFlag = 0 ) 
		throw() : Callback(_processor, _waitFlag)
	{
		memset( rspStr, 0, sizeof(rspStr) );
		strncpy( rspStr, _rspStr, MAX_AT_CMD_LEN );
	}

//	~AtCmdCallback()
//	{
//		DBG( "Destructor of AtCmdCallback %s is called, cbData is %s.", 
//				name(), cbData ? "not empty" : "empty" );
//	}
	const char *name() const
	{ return rspStr; }
protected:
	char			rspStr[MAX_AT_CMD_LEN+1];
};

typedef AtCmdCallback* AtCmdCallbackPtr;
typedef std::map<std::string, AtCmdCallbackPtr> AtCmdCallbackTable;
typedef std::list<AtCmdCallbackPtr> AtCmdCallbackList;

// device list
typedef ThreadSafeList<ZigbeeDevicePtr> ZigbeeDeviceList;
extern ZigbeeDeviceList zigbeeDevList;


extern int zigbee_init( char *dev_name, int baudrate, int flow_ctrl, 
			int databits, int stopbits, int parity );
extern int zigbee_finalize();

extern int zigbee_send_data( CmdType_t cmdType, Address_t *dstAddress, const void *data, uint16 dataLen );
// at command, timeout = -1 means wait until finish, 0 no wait, >0 wait for timeout seconds
// only 1 at cmd can be sent at a specific time.
extern int zigbee_at_cmd( CmdType_t cmdType, Address_t *dstAddress, const char *cmdString, 
			int timeout = 0, void **result = 0, uint16 *resultLen = 0 );

// ZCL commands
extern ZclBase *createZcl( uint16 clusterID );
extern int zigbee_zcl_cmd( Address_t *dstAddress, uint8 srcep, uint8 dstep, 
		uint16 profileID, uint16 clusterID, uint8 zclControl, uint8 zclCmd, 
		uint8 zclSeqNO, const void *zclPayload = NULL, uint16 zclPayloadLen = 0, 
		int timeout = 0, void **result = 0, uint16 *resultLen = 0,
		uint16 zclManufacturerCode = 0 );

// helper funcs
//extern void make_address( Address_t *addr, AddrType_t addrType, ... );

// utils for serial port
extern int OpenSerial( int fd, const char* port);
extern int InitSerial(int fd,	int speed, int flow_ctrl, int databits, int stopbits, int parity);

// for testing
#ifdef _DEBUG
extern void testUtil();
#endif


#endif








