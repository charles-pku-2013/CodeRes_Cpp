#ifndef _ZDEV_H
#define _ZDEV_H

#include "zigbee_common.h"
#include "zcl.h"

#define ZCL_HA_DEVICEID_UBEC_PLUG                          0x04FD
#define ZCL_HA_DEVICEID_UEBC_AIR_MONITOR                   0x04FE
#define ZCL_HA_DEVICEID_UEBC_SIMPLE_PLUG                   0x04FF
#define ZCL_HA_DEVICEID_UNKNOWN								0xFFFF


class ZclBase;
typedef std::vector<ZclBase*> ClusterList;

//!! cannot use operator= to a Derived class obj like UbecPlug from Base class, clusters will be overwritten
class ZigbeeDevice {
	friend struct ZDevCmp;
protected:
	enum { CLUSTER_GEN_BASIC = 0 };
public:
	//!! for ubec, only support HA_PROFILEID, and using 0 endpoint
	ZigbeeDevice();
	ZigbeeDevice( uint16 devID );

	virtual ~ZigbeeDevice();
    
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
    
	const uint8 *macAddr() const
	{ return _macAddr; }

	void setMacAddr(const uint8 *macAddr)
	{ memcpy( _macAddr, macAddr, 8 ); }
	void setMacAddr( const char *macStr )
	{
		int			j;
		char		tmp[3];
		uint8		data;
		uint8 		*pDst = _macAddr;

//		DBG( "ZigbeeDevice::setMacAddr() %s", macStr );
        
		tmp[2] = 0;
		for( j = 0; j < 8; ++j, macStr += 2 ) {
			strncpy( tmp, macStr, 2 );
			data = (uint8)strtol(tmp, NULL, 16);
			*pDst++ = data;
		} // for

//		pr_mem( _macAddr, 8 );
	}

	int queryMacAddr( );

	int ping( int timeout = ZIGBEE_TIMEOUT );
    
	const char *name() const
    { return _name.c_str(); }
    void setName( const char *devName )
    { _name = devName; }
    
    int update( int timeout = ZIGBEE_TIMEOUT );
    
    // invoked in the working routine
    int queryInfo( bool queryMacAddr = false );

	int getWarningCode() const
	{ return warningCode; }
	
#ifdef _DEBUG
	virtual char *toString()
	{
		static char 			buf[1024];
		char 					*ptr = buf;

		ptr += sprintf( ptr, "Device %s:\n", _name.c_str() );
		ptr += sprintf( ptr, "Address:\t\t\t%04x\n", _addr );
		ptr += sprintf( ptr, "MacAddr:\t\t\t" );
		pr_mem_str( macAddr(), 8, ptr );
		sprintf( ptr, "\n" );

		return buf;
	}

	virtual void test();
#endif
public:
	virtual int processWarning( uint8 *msg );
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
	std::string					_name;
	int							warningCode;
protected:
	ClusterList 				clusters;
protected:
	int get_mode_id( int timeout = ZIGBEE_TIMEOUT );
	int get_manu_name( int timeout = ZIGBEE_TIMEOUT );
private:
	void constructHelper();
};

//!! we cannot get mac addr of a device on each condition, so mac addr cannot be used to 
// identify a device
typedef enum { ZDEV_FIND_BY_MAC = 0x01, 
				ZDEV_FIND_BY_ADDR = 0x01 << 1,
				ZDEV_FIND_BY_NAME = 0x01 << 2, 
				ZDEV_FIND_BY_PROFILE = 0x01 << 3, 
				ZDEV_FIND_BY_ENDPOINT = 0x01 << 4 } ZDevFinder_t;

struct ZDevCmp {
	ZDevCmp( ZDevFinder_t _method, uint16 _addr = 0, const uint8 *pMac = NULL, 
		const char *_name = NULL, uint16 _profileID = ZCL_HA_PROFILE_ID, uint8 ep = 0 )
		: method(_method), addr(_addr), macAddr(pMac), name(_name), 
			profileID(_profileID), endpoint(ep) {}

	bool operator() ( const ZigbeeDevicePtr pDev ) const
	{
		if( (method & ZDEV_FIND_BY_MAC) && memcmp(macAddr, pDev->macAddr(), 8) )
			return false;
		if( (method & ZDEV_FIND_BY_ADDR) && addr != pDev->addr() )
			return false;		
		if( (method & ZDEV_FIND_BY_NAME) && strcmp(name, pDev->name()) )
			return false;
		if( (method & ZDEV_FIND_BY_PROFILE) && profileID != pDev->profileId() )
			return false;
		if( (method & ZDEV_FIND_BY_ENDPOINT) && endpoint != pDev->endpoint() )
			return false;		

		return true;
	}

	ZDevFinder_t			method;
	uint16					addr;
	uint16					profileID;
	uint8					endpoint;
	const uint8				*macAddr;
	const char				*name;
};


class UbecPlug : public ZigbeeDevice {
public:
	enum { CLUSTER_ON_OFF = 1, CLUSTER_POWER_METER };
	// warning msg
	enum { WARN_TEMPERATURE_LOW = 0x01, WARN_TEMPERATURE_HIGH };
	enum { WARN_CO2_HIGH = 0x04 };
public:
	UbecPlug();
public:	
	virtual uint16 devID() const
	{ return _devID; }
public:
	int on( int timeout = ZIGBEE_TIMEOUT );
	int off( int timeout = ZIGBEE_TIMEOUT );
	int toggle( int timeout = ZIGBEE_TIMEOUT );
	bool getOnOffState() const;
    float getVoltage() const;
	float getCurrent() const;
	float getPower() const;
	float getEnergy() const;
	float getPowerFactor() const;


#ifdef _DEBUG
	virtual void test();

	virtual char *toString()
	{
		char 				*retStr = ZigbeeDevice::toString();
		char				*ptr = retStr + strlen(retStr);

		ptr += sprintf( ptr, "OnOffState: %s\n", getOnOffState()?"ON":"OFF" );
		ptr += sprintf( ptr, "Voltage: %0.2f\n", getVoltage() );
		ptr += sprintf( ptr, "Current: %0.2f\n", getCurrent() );
		ptr += sprintf( ptr, "Power: %0.2f\n", getPower() );
		ptr += sprintf( ptr, "Energy: %0.3f\n", getEnergy() );
		ptr += sprintf( ptr, "PowerFactor: %0.2f\n", getPowerFactor() );

		return retStr;
	}
#endif
};



class UbecAirMonitor : public ZigbeeDevice {
public:		// cluster index
	enum { CLUSTER_CO2_MEASUREMENT = 1, 
		    CLUSTER_TEMP_CONFIG,
		    CLUSTER_HUMIDITY_MEASUREMENT };
public:				// warnings
	enum { WARN_LOW_TEMP = (int)ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG << 16 | 0x0001,
			WARN_HIGH_TEMP = (int)ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG << 16 | 0x0002, 
			WARN_HIGH_CO2 = (int)ZCL_CLUSTER_ID_UBEC_CO2_MEASUREMENT << 16 | 0x0004 };
public:
	UbecAirMonitor();
	virtual uint16 devID() const
	{ return _devID; }
    
public:
	float getTemperature() const;
	float getMinTemperature() const;
	float getMaxTemperature() const;
    float getHumidity() const;
	float getMinHumidity() const;
	float getMaxHumidity() const;
	uint16 getCO2Concentration() const;

	float getLowTempThreshold() const;
	int setLowTempThreshold( float value, int timeout = ZIGBEE_TIMEOUT );
	float getHighTempThreshold() const;	
	int setHighTempThreshold( float value, int timeout = ZIGBEE_TIMEOUT );

public:
	virtual int processWarning( uint8 *msg );

#ifdef _DEBUG
public:
	virtual void test();

	virtual char *toString()
	{
		char				*retStr = ZigbeeDevice::toString();
		char				*ptr = retStr + strlen(retStr);

		ptr += sprintf( ptr, "CO2Concentration: %d\n", getCO2Concentration() );
		ptr += sprintf( ptr, "Temperature: %0.2f\n", getTemperature() );
		ptr += sprintf( ptr, "MinTemperature: %0.2f\n", getMinTemperature() );
		ptr += sprintf( ptr, "MaxTemperature: %0.2f\n", getMaxTemperature() );
		ptr += sprintf( ptr, "Humidity: %0.2f\n", getHumidity() );
		ptr += sprintf( ptr, "MinHumidity: %0.2f\n", getMinHumidity() );
		ptr += sprintf( ptr, "MaxHumidity: %0.3f\n", getMaxHumidity() );

		return retStr;
	}
#endif
};

class UbecSimplePlug : public ZigbeeDevice {
public:
	enum { CLUSTER_ON_OFF = 1 };
	// warning msg
	enum { WARN_TEMPERATURE_LOW = 0x01, WARN_TEMPERATURE_HIGH };
	enum { WARN_CO2_HIGH = 0x04 };
public:
	UbecSimplePlug();
public:	
	virtual uint16 devID() const
	{ return _devID; }
public:
	int on( int timeout = ZIGBEE_TIMEOUT );
	int off( int timeout = ZIGBEE_TIMEOUT );
	int toggle( int timeout = ZIGBEE_TIMEOUT );
	bool getOnOffState() const;

#ifdef _DEBUG
	virtual void test();

	virtual char *toString()
	{
		char				*retStr = ZigbeeDevice::toString();
		char				*ptr = retStr + strlen(retStr);

		ptr += sprintf( ptr, "OnOffState: %s\n", getOnOffState()?"ON":"OFF" );

		return retStr;
	}

#endif
};

#endif

