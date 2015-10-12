#ifndef _ZDEV_H
#define _ZDEV_H

#include "zigbee_common.h"
#include "zcl.h"


// profile ids
// ha
//#define ZCL_HA_PROFILE_ID             						0x0104
// devices
//#define ZCL_HA_DEVICEID_ON_OFF_SWITCH  						0x0000
//#define ZCL_HA_DEVICEID_UBEC_PLUG                          0x04FD
//#define ZCL_HA_DEVICEID_UEBC_AIR_MONITOR                   0x04FE
//#define ZCL_HA_DEVICEID_UEBC_SIMPLE_PLUG                   0x04FF
//#define ZCL_HA_DEVICEID_UNKNOWN								0xFFFF




class ZigbeeDevice : public RWLockable {
public:
	ZigbeeDevice();
	ZigbeeDevice( const ZigbeeAddr &_addr );
	virtual ~ZigbeeDevice();

	void setChannel( uint8 ch )
	{ channel = ch; }
	uint8 getChannel() const
	{ return channel; }

	void setState( uint8 _State )
	{ state = _State; }
	uint8 getState() const
	{ return state; }

	void setAddr( uint16 nwkAddr )
	{ addr.setNwkAddr(nwkAddr); }
	void setAddr( const void *macAddr )
	{ addr.setMacAddr(macAddr); }
	
//	ZigbeeAddr& getAddr()
//	{ return addr; }
	const ZigbeeAddr& getAddr() const
	{ return addr; }

	void setPanID( uint16 nwkAddr )
	{ panID.setNwkAddr(nwkAddr); }
	void setPanID( const void *macAddr )
	{ panID.setMacAddr(macAddr); }
	
	const ZigbeeAddr& getPanID() const
	{ return panID; }

	void setParent( ZigbeeDevice *_ParentDev )
	{ parent = _ParentDev; }
	ZigbeeDevice* getParent()
	{ return parent; }

	int addService( ZigbeeService *pService );

#ifdef _DEBUG
	virtual const char *toString() const
	{
		static char				buf[1024];
		char					*ptr = buf;

		ptr += sprintf( ptr, "Device nwkAddr=%04x, macAddr=", getAddr().getNwkAddr() );
		pr_mem_str( getAddr().getMacAddr(), 8, ptr );
		
		return buf;
	}
#endif
protected:
	uint8				channel;
	uint8				state; // can only get the state of coord
	ZigbeeAddr			addr;
	ZigbeeAddr			panID;
	ZigbeeDevice		*parent;
	ZigbeeServiceTable	srvTable;
};


class ZigbeeCoord : public ZigbeeDevice {
public:
	ZigbeeCoord() : ZigbeeDevice( ZigbeeAddr((uint16)0) )
	{ setParent(this); }
};




class ZigbeeDeviceCompare {
public:
	ZigbeeDeviceCompare() {}
	ZigbeeDeviceCompare( const ZigbeeAddr &_addr )
	{ pDev.reset(new ZigbeeDevice(_addr)); }

	bool operator() ( const ZigbeeDevicePtr pLeft, const ZigbeeDevicePtr pRight ) const
	{ return pLeft->getAddr() < pRight->getAddr(); }

	operator ZigbeeDevicePtr()
	{ return pDev; }
protected:
	ZigbeeDevicePtr			pDev;
};


class ZigbeeService : public RWLockable {
public:
	ZigbeeService( const ZigbeeDevice *_pDev, uint8 ep, uint16 _profileID );
	virtual ~ZigbeeService();

	ZigbeeDevice *getDevice() const
	{ return pDev; }
	uint8 getEndpoint() const
	{ return endpoint; }
	uint16 getProfileID() const
	{ return profileID; }

	void setDevID( uint16 _devID )
	{ devID = _devID; }
	uint16 getDevID() const
	{ return devID; }

	void setDevVer( uint8 _devVer )
	{ devVer = _devVer; }
	uint8 getDevVer() const
	{ return devVer; }

//	ZigbeeAddr& getAddr()
//	{ return pDev->getAddr(); }	
	const ZigbeeAddr& getAddr() const
	{ return pDev->getAddr(); }	

	int addCluster( ZclPtr pCluster );
	int addClusterByID( uint16 clusterID );	

#ifdef _DEBUG
	const char *toString() const
	{
		static char 			buf[1024];
		char					*ptr = buf;

		ptr += sprintf( ptr, "ZigbeeService:\t\t\t%s:%02x\n", getAddr().toString(), getEndpoint() );
		ptr += sprintf( ptr, "ProfileID:\t\t\t%04x\n", getProfileID() );

		return buf;
	}
#endif
protected:
	uint8			endpoint;
	uint8			devVer;
	uint16			profileID;
	uint16			devID;
	ZigbeeDevice 	*pDev;
	ClusterSet 		clusters;
};




class ZigbeeServiceCompare {
public:
	ZigbeeServiceCompare()  {}
//	ZigbeeServiceCompare( const ZigbeeServiceCompare &rhs )
//	{
//		const ZigbeeServiceFinder *ptr = dynamic_cast<const ZigbeeServiceFinder*>(rhs.pService.get());
//		pService.reset( new ZigbeeServiceFinder(*ptr) );
//	}
	ZigbeeServiceCompare( const ZigbeeAddr &addr, uint8 ep ) 
	{
		pDev.reset( new ZigbeeDevice(addr) );
		pService.reset( new ZigbeeService(pDev.get(), ep, 0) );
	}
	
	operator ZigbeeServicePtr()
	{ return pService; }

	bool operator() ( const ZigbeeServicePtr pLeft, const ZigbeeServicePtr pRight ) const
	{
		if( pLeft->getAddr() != pRight->getAddr() )
			return pLeft->getAddr() < pRight->getAddr();
		
		return pLeft->getEndpoint() < pRight->getEndpoint();
	}
private:
	ZigbeeServicePtr						pService;
	boost::scoped_ptr<ZigbeeDevice> 		pDev;
};



//class ZigbeeServiceCompare : public ZigbeeService {
//public:
//	ZigbeeServiceCompare() : ZigbeeService(NULL, 0, 0) {}
//	ZigbeeServiceCompare( const ZigbeeAddr &addr, uint8 ep ) 
//				: ZigbeeService(new ZigbeeDevice, ep, 0)
//	{
//		apDev.reset( pDev );
//		apDev->setAddr( addr );
//	}
//	ZigbeeServiceCompare( const ZigbeeServiceCompare &rhs )
//				: ZigbeeService( rhs.getDevice(), rhs.getEndpoint(), rhs.getProfileID() )
//	{
//	}
//	
//	operator ZigbeeServicePtr()
//	{ return this; }

//	bool operator() ( const ZigbeeServicePtr pLeft, const ZigbeeServicePtr pRight ) const
//	{
//		if( pLeft->getAddr() != pRight->getAddr() )
//			return pLeft->getAddr() < pRight->getAddr();
//		
//		return pLeft->getEndpoint() < pRight->getEndpoint();
//	}
//private:
//	std::auto_ptr<ZigbeeDevice>			apDev;
//};




class ZigbeeServiceSet : public ThreadSafePtrSet<ZigbeeServicePtr, ZigbeeServiceCompare> {
public:
	void remove_by_dev( const ZigbeeDevice* pDev )
	{
		AutoWRLock lock(this);
		Iterator iter;
		Iterator it = this->lower_bound( ZigbeeServiceCompare(pDev->getAddr(), 0) );
		while( this->end() != it && (*it)->getAddr() == pDev->getAddr() ) {
			iter = it++;
//			delete *iter;
			this->erase( iter ); // in C++ 99, set::erase returns void
		} // while

		return;
	}
};



// global ZigbeeService set, indexed first by Addr, that is ZigbeeDevice, then by endpoints
extern ZigbeeServiceSet zigbeeSrvSet;
// global ZigbeeDevice set, indexed by Addr
extern ThreadSafePtrSet<ZigbeeDevicePtr, ZigbeeDeviceCompare> zigbeeDevSet;


#endif

