#ifndef _ZIGBEE_COMMON_H
#define _ZIGBEE_COMMON_H

#define	ZIGBEE_VERSION		"0.1.1-0608d"

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
#include <set>
#include <vector>
#include <memory>
//#include <boost/smart_ptr.hpp>


#ifdef _DEBUG
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
#endif


// types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef short int16;


inline
uint16 to_u16( const void *ptr )
{
	uint16 retval;
	memcpy( &retval, ptr, 2 );
	return retval;
}

inline
uint32 to_u32( const void *ptr )
{
	uint32 retval;
	memcpy( &retval, ptr, 4 );
	return retval;
}

inline
int16 to_16( const void *ptr )
{
	int16 retval;
	memcpy( &retval, ptr, 2 );
	return retval;
}



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
				fprintf( zigbeelog1, "%02x ", *((uint8*)(ptr) + i) );		\
			fputc('\n', zigbeelog1);					\
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
	extern char 		dbgbuf[1024];
	extern char 		dbgbuf2[1024];
	extern std::auto_ptr<log4cplus::Logger> 		dbgLogger;
	extern std::auto_ptr<log4cplus::Logger>			dbgLogger2;

	#define DBG( fmt, ... ) do {			\
						sprintf( dbgbuf, fmt"\n", ##__VA_ARGS__ );		\
						LOG4CPLUS_DEBUG(*dbgLogger, dbgbuf); 	\
				} while(0)
	#define DBG2( fmt, ... ) do { 						\
						sprintf( dbgbuf, fmt, ##__VA_ARGS__ );		\
						LOG4CPLUS_DEBUG(*dbgLogger, dbgbuf); 	\
				} while(0)
	#define pr_mem_str( ptr, len, str ) do {			\
					int i;							\
					for( i = 0; i < (len); ++i )		\
						(str) += sprintf( (str), "%02x ", *((uint8*)(ptr) + i) );		\
				}while(0)			
	#define pr_mem( ptr, len ) do { 				\
					char		*str = dbgbuf;		\
					pr_mem_str( ptr, len, str );	\
					LOG4CPLUS_DEBUG(*dbgLogger, dbgbuf); 	\
				} while(0)
	#define pr_mem2( ptr, len ) do { 			\
				char		*str = dbgbuf2;		\
				pr_mem_str( ptr, len, str );	\
				LOG4CPLUS_DEBUG(*dbgLogger2, dbgbuf2);	\
			}while(0)
//	#define pr_mem2_ascii( ptr, len ) do { 			\
//				int i;							\
//				for( i = 0; i < (len); ++i )		\
//					if( isprint(*((char*)(ptr) + i)) )		\
//						fprintf( zigbeelog2, "%c", *((char*)(ptr) + i) );		\
//					else													\
//						fprintf( zigbeelog2, "%02x ", *((uint8*)(ptr) + i) );		\
//			}while(0)
#else
	#define DBG
	#define DBG2
	#define pr_mem
	#define pr_mem2
	#define pr_mem_str
#endif

#define HA_PROFILEID		0x0104
#define ZIGBEE_TIMEOUT 		5
#define ZIGBEE_TRYCOUNT		3
#define ZIGBEE_BUFLEN 		512
#define MAX_AT_CMD_LEN		15

// profile ids
// ha
#define ZCL_HA_PROFILE_ID             						0x0104
// devices
#define ZCL_HA_DEVICEID_ON_OFF_SWITCH  						0x0000
// cluster ids
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_POWER_CFG                         0x0001
#define ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG                0x0002
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_GEN_ALARMS                            0x0009
#define ZCL_CLUSTER_ID_UBEC_POWERMETER						0x04FD
#define ZCL_CLUSTER_ID_UBEC_CO2_MEASUREMENT					0x04FE
#define ZCL_CLUSTER_ID_UBEC_TEMP_MEASUREMENT				0x0402
#define ZCL_CLUSTER_ID_UBEC_HUMIDITY_MEASUREMENT			0x0405

// special seqNO
#define SEQNO_QUERYINFO			0xFF
#define	SEQNO_UPDATE_DEV_LIST	0xFE
#define	SEQNO_CONFIG_REPORT		0xFD


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

typedef enum {
	COORD, ROUTER, ENDDEV
} ZigbeeDevType_t;

// find whether two obj like callback or device is equal, for find_if()
typedef enum { 
	CMPBYADDR = 0x01, CMPBYPROFILE = 0x01 << 1, CMPBYCLUSTER = 0x01 << 2,
	CMPBYSRCEP = 0x01 << 3, CMPBYDSTEP = 0x01 << 4, CMPBYSEQNO = 0x01 << 5,
	CMPBYCMD = 0x01 << 6,
	CMPBYALL = (0x01 << 7) - 1	//!! should not use ~0
} ZigbeeCmpType_t;


/* event handler */
typedef enum {
    ZIGBEE_NEW_JOIN,
    ZIGBEE_LEAVE,
    ZIGBEE_WARNING
} ZigbeeEventType;

class ZigbeeDevice;
typedef ZigbeeDevice* ZigbeeDevicePtr;
typedef int (*ZigbeeEventCallback)( ZigbeeEventType eventType, ZigbeeDevice *pDev );

extern ZigbeeEventCallback eventHandler;

typedef std::basic_string<uint8> PayloadType;

// zcl classes forward declaration
class ZclFrame;
struct ZclAttr;
struct ZclCmd;
class ZclBase;



// global vars declaration
extern int zigbee_fd;
//extern ZigbeeDevice thisDev;
extern std::auto_ptr<ZigbeeDevice> thisDev;


/* Data Structures */
class thread_exception : public std::exception {
public:
	thread_exception( const char *_msg ) throw() : msg(_msg) {}
	
	virtual const char* what() const throw()
	{ return msg; }
protected:
	const char *msg;
};


// list for holding pointers
template < typename T >
class PtrList : public std::list<T> {
	typedef typename std::list<T> BaseType;
public:
	typedef typename BaseType::iterator Iterator;
	typedef typename BaseType::value_type ValueType;
public:
    void add( const T &value )
    {
//        DBG( "PtrList::add()" );
        this->push_back( value );
    }
    
    template < typename Predicate >
    T find( const Predicate &pred )
    {
        Iterator		iter;

//		DBG( "PtrList::find() size = %u", this->size() );
        iter = std::find_if( this->begin(), this->end(), pred );

//		DBG( "PtrList::find() finish" );
        return (this->end() == iter ? (T)NULL : *iter);
    }
    
    template < typename Predicate >
    T find_remove( const Predicate &pred )
    {
        Iterator		iter;
        T               retval;
        
        iter = std::find_if( this->begin(), this->end(), pred );
        if( this->end() == iter )
            retval = (T)NULL;
        else {
            retval = *iter;
            this->erase( iter );
        }
        
        return retval;
    }

	
	template < typename Predicate >
	void add_unique_replace( const T &value, const Predicate &pred )
	{
		Iterator		iter;
		T				old;

		iter = std::find_if( this->begin(), this->end(), pred );
		if( this->end() != iter ) {
			// use new to replace the old
			old = *iter;
			*iter = value;
			delete old;
		} else {
			this->push_back( value );
		}

		return;
	}
	
	template < typename Predicate >
	int add_unique_no_replace( const T &value, const Predicate &pred )	
	{
		Iterator		iter;
		
		iter = std::find_if( this->begin(), this->end(), pred );
		if( this->end() != iter )
			return 1;
	
		this->push_back( value );
		return 0;
	}
    
	void clear()
	{
//		DBG( "PtrList::clear()" );
		for( Iterator it = this->begin(); it != this->end(); ++it )
			delete *it;
		BaseType::clear();
	}
};

template < typename T >
class ThreadSafePtrList : public PtrList<T> {
	typedef PtrList<T> BaseType;
	typedef int (*OperationFunc)( T data, void *arg );	
public:
	typedef typename BaseType::Iterator Iterator;
	typedef typename BaseType::ValueType ValueType;
public:
	ThreadSafePtrList() throw()
	{
		int err;
		err = pthread_mutex_init( &lock, NULL );
		if( err )
			throw thread_exception( "ThreadSafePtrList constructor init lock error" );
	}
	
	~ThreadSafePtrList()
	{
		pthread_mutex_destroy( &lock );
	}

	int add( const T &value )
	{
		int             ret;

//		DBG( "ThreadSafePtrList::add() invoked." );
		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto RET;
        
        BaseType::add( value );
        
RET:
		pthread_mutex_unlock( &lock );
		return ret;
	}

	template < typename Predicate >
	int add_unique_no_replace( const T &value, const Predicate &pred )
	{
		int				ret;
		Iterator		iter;

		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto END;

		ret = BaseType::add_unique_no_replace( value, pred );
		
END:		
		pthread_mutex_unlock( &lock );
		return ret;
	}

	template < typename Predicate >
	int add_unique_replace( const T &value, const Predicate &pred )
	{
		int 			ret;

		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto END;

		BaseType::add_unique_replace( value, pred );
END:		
		pthread_mutex_unlock( &lock );
		return ret;
	}
    
 
    template < typename Predicate >
	T find( const Predicate &pred )
    {
        int				err;
		T               retval = (T)0;

//		DBG( "ThreadSafePtrList::find() size = %u", this->size() );
        err = pthread_mutex_lock( &lock );
		if( err )
			goto END;
        
        retval = BaseType::find( pred );
        
END:
		pthread_mutex_unlock( &lock );		
//		DBG( "ThreadSafePtrList::find() finish" );
        return retval;
    }

	
	//!! can only be used for pointers data type
	template < typename Predicate >
	T find_remove( const Predicate &pred )
	{
		int				err;
		T               retval = (T)0;
		
		err = pthread_mutex_lock( &lock );
		if( err )
			goto END;
		
        retval = BaseType::find_remove( pred );

END:		
		pthread_mutex_unlock( &lock );
//		DBG( "list size is %u", this->size() );		
		return retval;
	}

	template < typename Predicate >
	int find_op( const Predicate &pred, OperationFunc op, void *arg )
	{
		int 			ret;
		T				target;
		
		ret = pthread_mutex_lock( &lock );
		if( ret )
			goto END;
		
		target = BaseType::find( pred );
		if( !target ) {
			ret = -2;
			goto END;
		}

		ret = op( target, arg );		
END:
		pthread_mutex_unlock( &lock );
		return ret;
	}

	void getCopy( std::vector<T> &copyList )
	{		
		int err;
		
		err = pthread_mutex_lock( &lock );
		if( err )
			goto END;
		
		copyList.assign( this->begin(), this->end() );
END:
		pthread_mutex_unlock( &lock );
	}

	void clear()
	{
//		DBG( "ThreadSafePtrList::clear()" );
	
		pthread_mutex_lock( &lock );
		BaseType::clear();	
		pthread_mutex_unlock( &lock );	
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


#if 0
template < typename T, typename Compare = std::less<T> >
class ThreadSafeSet : public std::set<T, Compare> {
	typedef typename std::set<T, Compare> BaseType;
public:
	typedef typename BaseType::iterator Iterator;
	typedef typename std::pair<Iterator, bool> InsertRetType;
public:
	InsertRetType insert( const T &value )
	{
		int err;
		InsertRetType retval;
		
		err = pthread_mutex_lock( &lock );
		
		if( err )
			throw thread_exception( "set lock error" );

		retval = BaseType::insert( value );

		pthread_mutex_unlock( &lock );

		return retval;
	}

	void clear()
	{
		int err;

		err = pthread_mutex_lock( &lock );
		
		if( err )
			throw thread_exception( "set lock error" );

		BaseType::clear();
		
		pthread_mutex_unlock( &lock );
	}
protected:
	pthread_mutex_t lock;
};
#endif // 0


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
		} // if 
	}

	virtual ~Callback()
	{
        finish( FAIL, -1 );
        
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
//		DBG( "Callback::wait() fail, %d", ret );
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
//#ifdef _DEBUG
//public:
//	virtual void print()
//	{
//		printf( "Callback info:\n" );		
//	}
//#endif
};

typedef Callback* CallbackPtr;
//typedef std::list<CallbackPtr> CallbackList;

// GeneralCallback including zcl and zdp that containing profile id, cluster id and endpoints
//!! who takes the responsibility of deleting, if waitFlag is set, the caller will delete it, either
// on success return or fail like timeout, otherwise, the processor func will delete it after calling its "finish"
class GeneralCallback : public Callback {
	friend struct GeneralCallbackCmp;
public:
	GeneralCallback( const Address_t *_addr, uint16 _profileID,
		uint16 _clusterID, uint8 _srcep, uint8 _dstep, uint8 _seqNO,
		CallbackProcessorFunc _processor, uint8 _waitFlag = 0, uint8 _cmdid = 0 ) 
		 : Callback(_processor, _waitFlag), profileID(_profileID), 
		clusterID(_clusterID), srcep(_srcep), dstep(_dstep), seqNO(_seqNO), 
		cmdID(_cmdid)
	{
		if( _addr )
			addr = *_addr;
	}

#ifdef _DEBUG
	virtual char* toString()
	{
		static char			buf[256];
		char				*ptr = buf;

		ptr += sprintf( ptr, "GeneralCallback seqNO\t\t\t %02x\n", seqNO );
		ptr += sprintf( ptr, "Adress:\t\t\t%04x:%02x->%02x\n", addr.addrData.shortAddr, srcep, dstep );
		ptr += sprintf( ptr, "Content:\t\t\t%04x:%04x:%02x\n", profileID, clusterID, cmdID );

		return buf;
	}
#endif
	
protected:
	Address_t			addr;
	uint8				seqNO;
	uint16				profileID;
	uint16				clusterID;
	uint8				srcep;
	uint8				dstep;
	uint8				cmdID;		// expected to return back

//#ifdef _DEBUG
//public:
//	virtual void print()
//	{
//		printf( "GeneralCallback info:\n" );
//		printf( "Address: " );
//		printAddr( &addr );
//		printf( "seqNO: %d\n", seqNO );
//		printf( "profileID: %04x\n", profileID );
//		printf( "clusterID: %04x\n", clusterID );
//		printf( "srcep: %02x\n", srcep );
//		printf( "dstep: %02x\n", dstep );
//	}
//#endif
}; 

typedef GeneralCallback* GeneralCallbackPtr;
typedef PtrList<GeneralCallbackPtr> GeneralCallbackList;
typedef ThreadSafePtrList<GeneralCallbackPtr> GeneralCallbackSafeList;


// for finding in list find_if()
struct GeneralCallbackCmp {
	GeneralCallbackCmp( ZigbeeCmpType_t _method = CMPBYALL, 
			Address_t *_addr = 0, uint16 _profileID = 0, 
			uint16 _clusterID = 0, uint8 _srcep = 0, uint8 _dstep = 0, 
			uint8 _seqNO = 0, uint8 _cmdid = 0 ) 
			: profileID(_profileID), clusterID(_clusterID), srcep(_srcep), 
			dstep(_dstep), seqNO(_seqNO), cmdID(_cmdid), method(_method)
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
		if( (method & CMPBYCMD) && (cmdID != ptr->cmdID) )
			return false;
		
		return true;
	}
	
	Address_t			addr;
	uint8				seqNO;
	uint16				profileID;
	uint16				clusterID;
	uint8				srcep;
	uint8				dstep;
	uint8				cmdID;

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
typedef PtrList<AtCmdCallbackPtr> AtCmdCallbackList;

// device list
typedef ThreadSafePtrList<ZigbeeDevicePtr> ZigbeeDeviceList;
extern ZigbeeDeviceList zigbeeDevList;


extern int zigbee_init( char *dev_name, int baudrate, int flow_ctrl, 
			int databits, int stopbits, int parity );
extern int zigbee_finalize();
extern void zigbee_get_dev_list( std::vector<ZigbeeDevicePtr> &devlist );
extern void zigbee_register_event_callback( ZigbeeEventCallback callbackFunc );
extern int zigbee_update_dev_list( bool clearOpt = true );
extern void zigbee_remove_dev( const ZigbeeDevicePtr pDev );


// callback processors
extern int parseDevQueryInfoQueryMac( void *arg, Callback *cb );
extern int parseDevQueryInfoNoMac( void *arg, Callback *cb );


//extern int zigbee_send_data( CmdType_t cmdType, Address_t *dstAddress, const void *data, uint16 dataLen );
// at command, timeout = -1 means wait until finish, 0 no wait, >0 wait for timeout seconds
// only 1 at cmd can be sent at a specific time.
extern int zigbee_at_cmd( CmdType_t cmdType, Address_t *dstAddress, const char *cmdString, 
			int timeout = 0, void **result = 0, uint16 *resultLen = 0 );

// ZCL commands
//extern ZclBase *createZcl( uint16 clusterID );
extern uint8 make_address( Address_t *addr, AddrType_t addrType, const void *data );
extern int zigbee_zcl_cmd( Address_t *dstAddress, uint8 srcep, uint8 dstep,
                          uint16 profileID, uint16 clusterID, const ZclFrame *pZclFrame,                           
                          int timeout = 0, void **result = 0, uint16 *resultLen = 0, 
                          CallbackProcessorFunc cbProcessor = 0, uint8 cmdID = 0 );
//extern int zigbee_zcl_cmd( Address_t *dstAddress, uint8 srcep, uint8 dstep, 
//		uint16 profileID, uint16 clusterID, uint8 zclControl, uint8 zclCmd, 
//		uint8 zclSeqNO, const void *zclPayload = NULL, uint16 zclPayloadLen = 0, 
//		int timeout = 0, void **result = 0, uint16 *resultLen = 0,
//		CallbackProcessorFunc cbProcessor = 0,
//		uint16 zclManufacturerCode = 0 );


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








