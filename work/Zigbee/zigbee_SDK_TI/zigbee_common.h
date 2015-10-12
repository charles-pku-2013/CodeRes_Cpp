#ifndef _ZIGBEE_COMMON_H
#define _ZIGBEE_COMMON_H

#define	ZIGBEE_VERSION		"0.0.8"

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
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <boost/smart_ptr.hpp>

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




#ifdef _DEBUG
extern FILE *zigbeelog1;
extern FILE *zigbeelog2;

extern const char *zigbeelog1_filename;
extern const char *zigbeelog2_filename;

extern uint8 zigbee_role_type;

#define COORD_ENDPOINT		10
#define ENDDEV_ENDPOINT		11

//#ifdef ZIGBEE_COORDINATOR
//#define ZIGBEE_LOG1 "/tmp/zigbeeCoordLog1"
//#define ZIGBEE_LOG2 "/tmp/zigbeeCoordLog2"
//#define ENDPOINT								0x0b
//#else
//#define ZIGBEE_LOG1 "/tmp/zigbeeEnddevLog1"
//#define ZIGBEE_LOG2 "/tmp/zigbeeEnddevLog2"
//#define ENDPOINT								0x0c
//#endif //ZIGBEE_COORD

#endif //_DEBUG

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
	#define DBG( fmt, ... ) fprintf( zigbeelog1, fmt"\n", ##__VA_ARGS__ )
	#define DBG2( fmt, ... ) fprintf( zigbeelog1, fmt, ##__VA_ARGS__ )
	#define pr_mem( ptr, len ) print_mem( ptr, len )
	#define pr_mem2( ptr, len ) do { 			\
				int i;							\
				for( i = 0; i < (len); ++i )		\
					fprintf( zigbeelog2, "%02x ", *((uint8*)(ptr) + i) );		\
			}while(0)
	#define pr_mem2_ascii( ptr, len ) do { 			\
				int i;							\
				for( i = 0; i < (len); ++i )		\
					if( isprint(*((char*)(ptr) + i)) )		\
						fprintf( zigbeelog2, "%c", *((char*)(ptr) + i) );		\
					else													\
						fprintf( zigbeelog2, "%02x ", *((uint8*)(ptr) + i) );		\
			}while(0)
	
	#define pr_mem_str( ptr, len, str ) do {			\
				int i;							\
				for( i = 0; i < (len); ++i )		\
					(str) += sprintf( (str), "%02x ", *((uint8*)(ptr) + i) );		\
			}while(0)
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
#define ZIGBEE_BUFLEN 		256


// special seqNO
#define SEQNO_QUERYINFO			0xFF
#define	SEQNO_UPDATE_DEV_LIST	0xFE
#define	SEQNO_CONFIG_REPORT		0xFD


// cmd NO
#define INVALID_CMD						0x0060

#define AF_INCOMING_MSG					0x8144
#define AF_DATA_REQUEST					0x0124
#define AF_DATA_REQUEST_SRSP			0x0164
#define AF_DATA_CONFIRM					0x8044
#define AF_REGISTER						0x0024
#define AF_REGISTER_SRSP				0x0064
#define SYS_RESET_REQ					0x0041
#define SYS_RESET_IND					0x8041
#define SYS_VERSION						0x0221
#define SYS_VERSION_SRSP				0x0261
#define SYS_OSAL_NV_READ				0x0821
#define SYS_OSAL_NV_READ_SRSP 			0x0861
#define SYS_OSAL_NV_WRITE				0x0921
#define SYS_OSAL_NV_WRITE_SRSP 			0x0961

#define ZDO_STARTUP_FROM_APP			0x4025
#define ZDO_STARTUP_FROM_APP_SRSP		0x4065
#define ZDO_SET_LINK_KEY				0x2345
#define ZDO_SET_LINK_KEY_SRSP			0x2365
#define ZDO_ACTIVE_EP_REQ				0x0525
#define ZDO_ACTIVE_EP_REQ_SRSP			0x0565
#define ZDO_SIMPLE_DESC_REQ				0x0425
#define ZDO_SIMPLE_DESC_REQ_SRSP		0x0465
#define ZDO_NWK_DISCOVERY_REQ			0x2625
#define ZDO_NWK_DISCOVERY_REQ_SRSP		0x2665
#define ZDO_STATE_CHANGE_IND			0xC045
#define ZDO_END_DEVICE_ANNCE_IND		0xC145


#define ZB_GET_DEVICE_INFO				0x0626
#define ZB_GET_DEVICE_INFO_SRSP			0x0666




// config IDs
// nv_configid
#define ZCD_NV_STARTUP_OPTION			0x0003	
#define ZCD_NV_LOGICAL_TYPE 			0x0087
#define ZCD_NV_PANID        			0x0083
#define ZCD_NV_CHANLIST					0x0084
#define ZCD_NV_ZDO_DIRECT_CB			0x008F		// enable callback funcs

// startup clear opt
#define STARTOPT_CLEAR_CONFIG			0x01
#define STARTOPT_CLEAR_STATE			0x02


// return status
#define NV_ITEM_UNINIT					0x09


// device state
#define DEV_ZB_COORD					0x09
#define DEV_ZB_ENDDEV					0x06


#define DEVICEVERSION       		0


/* Data Structures */
class thread_exception : public std::exception {
public:
	thread_exception( const char *_msg ) throw() : msg(_msg) {}
	
	virtual const char* what() const throw()
	{ return msg; }
protected:
	const char *msg;
};


class RWLockable {
public:
	RWLockable() throw()
	{
		int			err;
		
		err = pthread_rwlock_init( &lock, NULL );
		if( err ) {
			errno = err;
			throw thread_exception( "init lock error" );
		}
	}

	virtual ~RWLockable()
	{ pthread_rwlock_destroy( &lock ); }

	void setRDLock() throw()
	{
		int 		err;
		
		err = pthread_rwlock_rdlock( &lock );
		if( err ) {
			errno = err;
			throw thread_exception( "set read lock error" );
		}		
	}

	void setWRLock() throw()
	{
		int 		err;
		
		err = pthread_rwlock_wrlock( &lock );
		if( err ) {
			errno = err;
			throw thread_exception( "set write lock error" );
		}		
	}	

	void releaseLock() throw()
	{
		int 		err;
		
		err = pthread_rwlock_unlock( &lock );
		if( err ) {
			errno = err;
			throw thread_exception( "release lock error" );
		}
	}
protected:
	pthread_rwlock_t		 lock;
};


class AutoRDLock {
public:
	AutoRDLock( RWLockable *obj ) : lockableObj(obj)
	{ lockableObj->setRDLock(); }
	~AutoRDLock()
	{ lockableObj->releaseLock(); }
private:
	RWLockable		*lockableObj;
};

class AutoWRLock {
public:
	AutoWRLock( RWLockable *obj ) : lockableObj(obj)
	{ lockableObj->setWRLock(); }
	~AutoWRLock()
	{ lockableObj->releaseLock(); }
private:
	RWLockable		*lockableObj;
};


// list for holding boost::shared_ptr
template < typename T >
class PtrList : public std::list<T> {
	typedef typename std::list<T> BaseType;
	typedef typename T::element_type DataType;
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
        return (this->end() == iter ? T((DataType*)NULL) : *iter);
    }
    
    template < typename Predicate >
    T find_remove( const Predicate &pred )
    {
        Iterator		iter;
        T               retval;
        
        iter = std::find_if( this->begin(), this->end(), pred );
        if( this->end() == iter )
            retval = T((DataType*)NULL);
        else {
            retval = *iter;
            this->erase( iter );
        }
        
        return retval;
    }

	
	template < typename Predicate >
	T add_unique_replace( const T &value, const Predicate &pred )
	{
		Iterator		iter;
		T				old((DataType*)NULL);

		iter = std::find_if( this->begin(), this->end(), pred );
		if( this->end() != iter ) {
			// use new to replace the old
			old = *iter;
			*iter = value;
		} else {
			this->push_back( value );
		}

		return old;
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
    
//	void clear()
//	{
//		for( Iterator it = this->begin(); it != this->end(); ++it )
//			delete *it;
//		BaseType::clear();
//	}
};



template < typename T >
class ThreadSafePtrList : public PtrList<T>, public RWLockable {
	typedef PtrList<T> BaseType;
	typedef int (*OperationFunc)( T data, void *arg );	
public:
	typedef typename BaseType::Iterator Iterator;
	typedef typename BaseType::ValueType ValueType;
public:
	void add( const T &value )
	{
		AutoWRLock lock( this );
        BaseType::add( value );
	}

	template < typename Predicate >
	int add_unique_no_replace( const T &value, const Predicate &pred )
	{
		AutoWRLock lock( this );		
		return BaseType::add_unique_no_replace( value, pred );		
	}

	template < typename Predicate >
	T add_unique_replace( const T &value, const Predicate &pred )
	{
		AutoWRLock lock( this );
		return BaseType::add_unique_replace( value, pred );
	}
    
 
    template < typename Predicate >
	T find( const Predicate &pred )
    {
		AutoRDLock lock( this );
        return BaseType::find( pred );
    }

	
	//!! can only be used for pointers data type
	template < typename Predicate >
	T find_remove( const Predicate &pred )
	{
		AutoWRLock lock( this );
        return BaseType::find_remove( pred );
	}

	template < typename Predicate >
	int find_op( const Predicate &pred, OperationFunc op, void *arg )
	{
		T				target;
		
		AutoRDLock lock( this );		
		target = BaseType::find( pred );
		if( !target )
			return -2;

		return op( target, arg );		
	}

	void getCopy( std::vector<T> &copyList )
	{		
		AutoRDLock lock( this );				
		copyList.assign( this->begin(), this->end() );
	}

//	void clear()
//	{
//		AutoWRLock lock( this );
//		BaseType::clear();	
//	}
};



template < typename T, typename Compare >
class ThreadSafePtrSet : public std::set<T, Compare>, public RWLockable {
	typedef typename std::set<T, Compare> BaseType;
	typedef int (*OperationFunc)( T data, void *arg );	
public:
	typedef typename BaseType::iterator Iterator;
	typedef typename std::pair<Iterator, bool> InsertRetType;
public:
	int add( const T &value )
	{
		AutoWRLock lock( this );
        InsertRetType ret = BaseType::insert( value );
		return !( ret.second );
	}

	int find_op( const T &key, OperationFunc op, void *arg )
	{
		AutoRDLock lock( this );

		Iterator it = this->find( key );
		
		if( this->end() == it )
			return -2;

		return op( *it, arg );
	}

	T find_remove( const T &key )
	{
		AutoWRLock lock( this );

		Iterator it = this->find( key );
		
		if( this->end() == it )
			return (T)NULL;

		this->erase( it );
		return *it;
	}

//	void clear()
//	{
//		AutoWRLock lock( this );

//		for( Iterator it = this->begin(); it != this->end(); ++it )
//			delete *it;

//		BaseType::clear();
//	}
};


typedef uint16			ZigbeeNwkAddr;
typedef uint8			ZigbeeMacAddr[8];


class ZigbeeAddr {
public:
	enum { NOT_PRESENT, HAS_NWKADDR, HAS_MACADDR };
public:
	ZigbeeAddr() : type(NOT_PRESENT), nwkAddrData(0)
	{ memset( macAddrData, 0, 8 ); }

	ZigbeeAddr( uint16 nwkAddr ) : type(HAS_NWKADDR), nwkAddrData(nwkAddr)
	{ memset( macAddrData, 0, 8 ); }

	ZigbeeAddr( const void *vptr ) : type(HAS_MACADDR), nwkAddrData(0)
	{ 
//		DBG2( "ZigbeeAddr MAC constructor" );
//		pr_mem( vptr, 8 );
		memcpy( macAddrData, vptr, 8 ); 
//		pr_mem( macAddrData, 8 );
	}

	void setNwkAddr( ZigbeeNwkAddr nwkAddr )
	{
		type |= HAS_NWKADDR;
		nwkAddrData = nwkAddr;
	}

	void setMacAddr( const void *vptr )
	{
		type |= HAS_MACADDR;
		memcpy( macAddrData, vptr, 8 );
	}

	void setAddr( ZigbeeNwkAddr nwkAddr )
	{ setNwkAddr( nwkAddr ); }

	void setAddr( const void *vptr )
	{ setMacAddr( vptr ); }

	ZigbeeNwkAddr getNwkAddr() const
	{ return nwkAddrData; }

	const ZigbeeMacAddr &getMacAddr() const
	{ return macAddrData; }

	// always nwkaddr first
	bool operator == ( const ZigbeeAddr &rhs ) const
	{
		if( (type & HAS_NWKADDR) && (rhs.type & HAS_NWKADDR) )
			return nwkAddrData == rhs.nwkAddrData;
		if( (type & HAS_MACADDR) && (rhs.type & HAS_MACADDR) )
			return memcmp( macAddrData, rhs.macAddrData, 8 ) == 0;
		if( (type & NOT_PRESENT) && (rhs.type & NOT_PRESENT) )
			return true;

		return false;
	}

	bool operator != ( const ZigbeeAddr &rhs ) const
	{
		return !(*this == rhs);
	}

	bool operator < ( const ZigbeeAddr &rhs ) const
	{
		if( (type & HAS_NWKADDR) && (rhs.type & HAS_NWKADDR) )
			return nwkAddrData < rhs.nwkAddrData;
		if( (type & HAS_MACADDR) && (rhs.type & HAS_MACADDR) )
			return memcmp( macAddrData, rhs.macAddrData, 8 ) < 0;

		return false;
	}

//#ifdef _DEBUG
//	void test() const
//	{
//		pr_mem( macAddrData, 8 );
//	}
//#endif

protected:
	uint8					type;
	ZigbeeNwkAddr			nwkAddrData;
	ZigbeeMacAddr			macAddrData;
#ifdef _DEBUG
public:
	const char *toString() const
	{
		static char		buf[15];
		char			*ptr = buf;

		if( HAS_NWKADDR == type )
			sprintf( buf, "%04x", this->nwkAddrData );
		else if( HAS_MACADDR == type )
			pr_mem_str( this->macAddrData, 8, ptr );
		else
			sprintf( buf, "NOT_PRESENT" );

		return buf;
	}
#endif
};




typedef enum {
	ZIGBEE_CMD_POLL,
	ZIGBEE_CMD_SREQ,
	ZIGBEE_CMD_AREQ,
	ZIGBEE_CMD_SRSP
} ZigbeeCmdType;


typedef enum {
	ZIGBEE_COORD, ZIGBEE_ROUTER, ZIGBEE_ENDDEV
} ZigbeeDevType;

/* event handler */
typedef enum {
    ZIGBEE_NEW_JOIN,
    ZIGBEE_LEAVE,
    ZIGBEE_WARNING
} ZigbeeEventType;


class ZigbeePayload {
	typedef std::basic_string<uint8> 	ContainerType;
	typedef ContainerType::size_type 	SizeType;
public:
	ZigbeePayload() {}
	ZigbeePayload( uint16 size )
	{ reserve(size); }
	ZigbeePayload( const uint8 *start, const uint8 *end )
	{ setData( start, end ); }	
	ZigbeePayload( const void *vptr, ContainerType::size_type len )
	{ setData( vptr, len ); }
    void setData( const uint8 *start, const uint8 *end )
    { _data.assign( start, end ); }
    void setData( const void *vptr, ContainerType::size_type len )
    {
    	const uint8 *ptr = (const uint8*)vptr;
    	_data.assign( ptr, len );
	}
    void appendData( const uint8 *start, const uint8 *end )
    { _data.append( start, end ); }
    void appendData( const void *vptr, ContainerType::size_type len )
    {
    	const uint8 *ptr = (const uint8*)vptr;
    	_data.append( ptr, len );
	}
    void appendData( uint8 val )
	{ _data.push_back(val); }
	void clear()
	{ _data.clear(); }
	void swap( ZigbeePayload rhs )
	{ _data.swap( rhs._data ); }

	void reserve( SizeType size )
	{ _data.reserve(size); }

    const uint8 *data() const
    { return (dataLen() ? _data.c_str() : NULL); }
    uint16 dataLen() const
    { return (uint16)_data.size(); }
protected:
	ContainerType			_data;
};



// forward dec
class ZclBase;
struct ZclAttr;
struct ZclCmd;
class ZclCompare;
typedef ZclBase* ZclPtr;
typedef std::set<ZclPtr, ZclCompare> ClusterSet;

class ZigbeeService;
typedef boost::shared_ptr<ZigbeeService> ZigbeeServicePtr;
typedef std::map<uint8, ZigbeeService*> ZigbeeServiceTable;

class ZigbeeDevice;
typedef boost::shared_ptr<ZigbeeDevice> ZigbeeDevicePtr;

typedef int (*ZigbeeEventCallback)( ZigbeeEventType eventType, ZigbeeDevice *pDev );
extern ZigbeeEventCallback eventHandler;




class Callback;
typedef int (*CallbackProcessorFunc)(Callback *pcb, void *arg1, void *arg2);

class Callback {
public:
	// for status
	typedef enum { FAIL = -1, SUCCESS = 0, WAITTING = 1 } StatusType;
	// free the memory of cb data, 2nd arg means whether array or not.
	typedef void (*DataFreeType)(void*, bool);
public:
	// waitFlag means whether we need the response of this cmd.
	Callback( CallbackProcessorFunc _processor, int _timeout ) 
		throw() : status(WAITTING), retval(0), processor(_processor), timeout(_timeout), 
		cbData(0), cbDataLen(0), cbDataFreeFunPtr(NULL),
		processorData(0), processorDataLen(0), processorDataFreeFunPtr(NULL)
	{
		int err;

		timeStamp = time(NULL);
		
		if( timeout ) {		
			err = pthread_mutex_init( &lock, NULL );
			if( err )
				throw thread_exception( "init lock error" );
			err = pthread_cond_init( &cond, NULL );
			if( err ) {
				pthread_mutex_destroy( &lock );
				throw thread_exception( "init cond var error" );
			}		
		} // if 
	}

	virtual ~Callback()
	{
        finish( FAIL, -1 );
        
		if( cbData && cbDataFreeFunPtr )
			cbDataFreeFunPtr( cbData, cbDataLen > 1 );

		if( processorData && processorDataFreeFunPtr )
			processorDataFreeFunPtr( processorData, processorDataLen > 1 );
		
		if( timeout ) {
			pthread_mutex_destroy( &lock );
			pthread_cond_destroy( &cond );
		}
	}

	// call by the func that issues the command
	// timeout in seconds, < 0 wait until finish, = 0 return immediately with success
	// if fail on timeout, errno need to be checked
	int wait()
	{
		int ret = 0;
		struct timeval	tv; 
		struct timespec ts;

		if( !timeout )
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

		status = WAITTING;			//!! restore

		pthread_mutex_unlock( &lock );
		return retval;
FAIL:
		pthread_mutex_unlock( &lock );
		errno = ret;
		DBG( "Callback::wait() fail, %d", ret );
		return -1;
	}

	void reset()
	{
		finish( FAIL, -1 );
		status = WAITTING;		
	}

	// call by the working thread, and wake up any waiters
	//!! before call it, we must check whether it exist, for it may be deleted for timeout
	//!! on timeout or abnormal return of wait, we should call this func on main thread
	// to prevent deleting-finish race condition
	void finish( StatusType _status, int _retval )
	{	
		if( timeout )
			pthread_mutex_lock( &lock );
		
		status = _status;
		retval = _retval;

		if( timeout ) {
			pthread_mutex_unlock( &lock );
			pthread_cond_signal( &cond );
		}
	}

	CallbackProcessorFunc setProcessor( CallbackProcessorFunc funptr )
	{ 
		CallbackProcessorFunc old = processor;
		processor = funptr;
		return old;
	}
	
	CallbackProcessorFunc getProcessor() const
	{ return processor; }
	int getTimeOut() const
	{ return timeout; }
	time_t getTimeStamp() const
	{ return timeStamp; }

	// cbDataLen, when type is void, means the num of bytes, otherwise, the num of elements
	void setCbData( void *ptr, DataFreeType freer, uint16 len = 1 )
	{		
		if( cbData && cbDataFreeFunPtr )
			cbDataFreeFunPtr( cbData, cbDataLen > 1 );		
		cbData = ptr;
		cbDataLen = len;
		cbDataFreeFunPtr = freer;
	}

	//!! onece this func called by the user, the Callback obj never own the cbData, it's now the
	// user's duty to free the cbData memory.
	void *getCbData( uint16 *pDataLen = NULL )
	{
		void *retVal = cbData;
		cbData = NULL;
		if( pDataLen )
			*pDataLen = cbDataLen;
		cbDataLen = 0;
		return retVal;
	}

	void setProcessorData( void *ptr, DataFreeType freer, uint16 len = 1 )
	{		
		if( processorData && processorDataFreeFunPtr )
			processorDataFreeFunPtr( processorData, processorDataLen > 1 );
		processorData = ptr;
		processorDataLen = len;
		processorDataFreeFunPtr = freer;
	}

	void *getProcessorData( uint16 *pDataLen = NULL )
	{
		void *retVal = processorData;
		processorData = NULL;
		if( pDataLen )
			*pDataLen = processorDataLen;
		processorDataLen = 0;
		return retVal;
	}	

	int runProcessor( void *arg1, void *arg2 )
	{
		if( processor )
			retval = processor( this, arg1, arg2 );
		return retval;
	}	

	int getRetVal() const
	{ return retval; }
	
protected:
	volatile StatusType		status;
	int						retval;			// return value of processor
	// processor func call by the working thread
	// it must change the status member
	CallbackProcessorFunc 		processor;
	// for notify through cond var
	int						timeout;
	pthread_mutex_t			lock;
	pthread_cond_t			cond;

	// for processor data
	void*					processorData;
	uint16					processorDataLen;
	DataFreeType			processorDataFreeFunPtr;
	
	// for callback data
	void*					cbData;
	uint16					cbDataLen;
	DataFreeType			cbDataFreeFunPtr;

	// timestamp	
	time_t				timeStamp;
};

typedef boost::shared_ptr<Callback> CallbackPtr;


// cmdID for ZCL and cmdNO for TI
class GeneralCallback : public Callback {
	friend class GeneralCallbackFinder;
public:
	GeneralCallback( uint16 _cmdNO = 0,
					const ZigbeeAddr &_addr = ZigbeeAddr(), 
					uint16 _profileID = 0,	uint16 _clusterID = 0, 
					uint8 _srcEp = 0, uint8 _dstEp = 0, 
					uint8 _seqNO = 0,
					CallbackProcessorFunc _processor = 0, int _timeout = ZIGBEE_TIMEOUT, 
					uint8 _cmdID = 0 )
		: Callback(_processor, _timeout),
		cmdNO(_cmdNO),
		addr(_addr),
		profileID(_profileID), clusterID(_clusterID), 
		srcEp(_srcEp), dstEp(_dstEp), 
		seqNO(_seqNO), 
		cmdID(_cmdID)
	{}

	uint16 getCmdNO() const
	{ return cmdNO; }

	void setSrcEp( uint8 ep )
	{ srcEp = ep; }
	uint8 getSrcEp() const
	{ return srcEp; }

	void setDstEp( uint8 ep )
	{ dstEp = ep; }
	uint8 getDstEp() const
	{ return dstEp; }

	void setSeqNO( uint8 seq )
	{ seqNO = seq; }
	uint8 getSeqNO() const
	{ return seqNO; }
	
#ifdef _DEBUG
	virtual const char* toString()
	{
		static char			buf[256];
		char				*ptr = buf;

		ptr += sprintf( ptr, "GeneralCallback seqNO\t\t\t %02x\n", seqNO );
		ptr += sprintf( ptr, "Adress:\t\t\t%s:%02x->%02x\n", addr.toString(), srcEp, dstEp );
		ptr += sprintf( ptr, "Content:\t\t\t%04x:%04x:%02x\n", profileID, clusterID, cmdID );

		return buf;
	}
#endif
	
protected:
	uint8				seqNO;
	uint8				srcEp;
	uint8				dstEp;
	uint8				cmdID;		// expected to return back	
	uint16				profileID;
	uint16				clusterID;
	uint16				cmdNO;
	ZigbeeAddr			addr;
}; 

typedef boost::shared_ptr<GeneralCallback> GeneralCallbackPtr;




// for finding in list find_if()
class GeneralCallbackFinder {
public:
	typedef enum { 
		BYADDR = 0x01, BYPROFILE = 0x01 << 1, BYCLUSTER = 0x01 << 2,
		BYSRCEP = 0x01 << 3, BYDSTEP = 0x01 << 4, BYSEQNO = 0x01 << 5,
		BYCMDID = 0x01 << 6, BYCMDNO = 0x01 << 7,
		BYALL = (0x01 << 8)- 1
	} MethodType;
public:
	GeneralCallbackFinder( MethodType _method,
					uint16 _cmdNO = 0,
					const ZigbeeAddr &_addr = ZigbeeAddr(), 
					uint16 _profileID = 0,	uint16 _clusterID = 0, 
					uint8 _srcEp = 0, uint8 _dstEp = 0, 
					uint8 _seqNO = 0,
					uint8 _cmdID = 0 )
			:  method(_method),
			cmdNO(_cmdNO),
			addr(_addr), 
			profileID(_profileID), clusterID(_clusterID), 
			srcEp(_srcEp), dstEp(_dstEp), 
			seqNO(_seqNO), 
			cmdID(_cmdID)
 	{}

	bool operator() ( const GeneralCallbackPtr &ptr ) const
	{
		if( (method & BYCMDNO) && (cmdNO != ptr->cmdNO) )
			return false;	
		if( (method & BYADDR) && (addr != ptr->addr) )
			return false;	
		if( (method & BYPROFILE) && (profileID != ptr->profileID) )
			return false;
		if( (method & BYCLUSTER) && (clusterID != ptr->clusterID) )
			return false;		
		if( (method & BYSRCEP) && (srcEp != ptr->srcEp) )
			return false;
		if( (method & BYDSTEP) && (dstEp != ptr->dstEp) )
			return false;
		if( (method & BYSEQNO) && (seqNO != ptr->seqNO) )
			return false;	
		if( (method & BYCMDID) && (cmdID != ptr->cmdID) )
			return false;
		
		return true;
	}

protected:	
	uint8				seqNO;
	uint8				srcEp;
	uint8				dstEp;
	uint8				cmdID;		// expected to return back	
	uint16				profileID;
	uint16				clusterID;
	uint16				cmdNO;
	ZigbeeAddr			addr;

	MethodType 			method;
};


// response to the SREQ commands
// All SREQ cmds must wait for their corresponding SRSP to finish
// There only one SRSP callback obj, that means we can only issue one cmd to dongle at a time.
class AREQCallback : public GeneralCallback {
public:
	AREQCallback( uint16 _cmdNO, 
				CallbackProcessorFunc _processor, int _timeout = ZIGBEE_TIMEOUT )
		: GeneralCallback( _cmdNO, ZigbeeAddr(), 0, 0, 0, 0, 0, _processor, _timeout )
	{}
};

//typedef boost::shared_ptr<CmdCallback> CmdCallbackPtr;

//class CmdSRSPCallback : public CmdCallback {
//public:
//	CmdSRSPCallback( uint16 _cmdNO, 
//				CallbackProcessorFunc _processor = NULL, int _timeout = ZIGBEE_TIMEOUT )
//		: CmdCallback(_cmdNO, _processor, _timeout)
//	{}
//};

// All other cmds except incomming data
//class CmdAREQCallback : public CmdCallback {
//public:
//	CmdAREQCallback( uint16 _cmdNO, const ZigbeeAddr &_addr, 
//				CallbackProcessorFunc _processor, int _timeout )
//		: CmdCallback(_cmdNO, _processor, _timeout), addr(_addr)
//	{}

//protected:
//	ZigbeeAddr		addr;
//};







// DS type for holding callback ptrs
class CallbackSafeList : public ThreadSafePtrList<CallbackPtr> {
public:
	template < typename Predicate >
	CallbackPtr find_remove( const Predicate &pred )
	{
		Iterator		it;
		CallbackPtr		retval = NULL;
		double			elapsed;
		time_t 			now = time(NULL);
	
		AutoWRLock lock( this );
		for( it = this->begin(); it != this->end(); ++it ) {
			elapsed = difftime( now, (*it)->getTimeStamp() );
			if( elapsed > (uint32)((*it)->getTimeOut()) ) {
//				delete *it;
				it = this->erase(it);	//!!
			} else if( pred(*it) ) {
				retval = *it;
				this->erase(it);
				return retval;
			} // if
		} // while

		return retval;
	}
};


typedef PtrList<CallbackPtr> CallbackList;

//typedef void (*CallbackParserFunc)( uint16 cmdNO, const uint8 *pStart, const uint8 *pEnd );
typedef std::map<uint16, GeneralCallbackPtr> CallbackParserTable;


// device list
typedef ThreadSafePtrList<ZigbeeDevicePtr> ZigbeeDeviceList;

extern int zigbee_fd;
extern std::auto_ptr<ZigbeeDevice> thisDev;


// func dec
extern int zigbee_init( char *dev_name, uint8 type, uint8 clearOpt, int baudrate, int flow_ctrl, 
			int databits, int stopbits, int parity );
//extern int zigbee_register_app( uint8 ep, uint16 profileID, uint16 devID, uint8 devVer, 
//			uint8 nInClusters, const uint16 *inClustersList, uint8 nOutClusters, const uint16 *outClusterList );
//extern int zigbee_start();
extern int zigbee_finalize();
extern void zigbee_get_dev_list( std::vector<ZigbeeDevicePtr> &devlist );
extern void zigbee_register_event_callback( ZigbeeEventCallback callbackFunc );
//extern int zigbee_update_dev_list();
extern int zigbee_set_link_key();
extern int zigbee_scan();


 

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








