#include "zigbee_common.h"
#include "zdev.h"
#include "zcl.h"



#ifdef _DEBUG

void ZigbeeDevice::test()
{
	DBG( "ZigbeeDevice::test()" );
}

void UbecPlug::test()
{
	using namespace std;

	DBG( "UbecPlug::test()" );

	DBG( "UbecPlug::test() end!" );
}


void UbecAirMonitor::test()
{
	DBG( "UbecAirMonitor::test()" );

//	{
//		this->setLowTempThreshold( 21.25, ZIGBEE_TIMEOUT );
//		this->update( ZIGBEE_TIMEOUT );
//		perror( "errno is:" );
//		float value = this->getLowTempThreshold();
//		printf( "%0.2f\n", value );
//	}

	// high temp warning test
	{
		float val;
		time_t now;
		this->update();
		val = this->getHighTempThreshold();
		now = time(NULL);
		printf( "%s HighTempThreshold before change is %0.2f.\n", ctime(&now), val );
		this->setHighTempThreshold( 25 );
		this->update();
		val = this->getHighTempThreshold();	
		now = time(NULL);
		printf( "%s HighTempThreshold after change is %0.2f.\n", ctime(&now), val );
	} // end high temp warning test


#if 0
	// test commission cluster
	{
		static const uint16 clusterID = 0x0015;
	
		// restart dev
//		ZclBase::issueZclCmd( *this, clusterID, 0x0, NULL, 0, 0 );

		ZclFrame		reqFrame;
		
		uint16			addrData = this->addr();
		Address_t		addr;
		uint16			attrID;
		
		make_address( &addr, SHORT, &addrData );	

		// read attr
		reqFrame.setSeqNO( this->seqNO() );
		reqFrame.setCmd( (uint8)0x0 );

		for( uint16 i = 0; i < 7; ++i ) {
			attrID = i;
			reqFrame.appendPayload( &attrID, 2 );
		}

		zigbee_zcl_cmd( &addr, thisDev->endpoint(), this->endpoint(), this->profileId(), clusterID, &reqFrame, 0 );		
	} // block
#endif

	DBG( "UbecAirMonitor::test() end" );
}

void UbecSimplePlug::test()
{
	DBG( "UbecSimplePlug::test()" );

	{
		ZclBase *pZcl = new ZclGenPowerCfg;
		pZcl->updateAllAttrs( *this, ZIGBEE_TIMEOUT );

		delete pZcl;
	}
	
	DBG( "UbecSimplePlug::test() end!" );
}

#endif //_DEBUG

// members of ZigbeeDevice
ZigbeeDevice::ZigbeeDevice() : _devID(ZCL_HA_DEVICEID_UNKNOWN), _profileId( HA_PROFILEID ), _endpoint(0), 
										_seqNO(0), _role(ENDDEV), warningCode(0)
{
	constructHelper();
}

ZigbeeDevice::ZigbeeDevice( uint16 devId ) : _devID(devId), _profileId( HA_PROFILEID ), 
				_endpoint(0), _seqNO(0), _role(ENDDEV), warningCode(0)
{
	constructHelper();
}

ZigbeeDevice::~ZigbeeDevice()
{
//	DBG( "ZigbeeDevice destructor of %04x", this->addr() );
	for( ClusterList::iterator it = clusters.begin(); it != clusters.end(); ++it )
		delete *it;
//	DBG( "ZigbeeDevice destructor of %04x finish.", this->addr() );
}


void ZigbeeDevice::constructHelper()
{
	memset( _macAddr, 0, sizeof(_macAddr) );
	clusters.resize(1);
	clusters[CLUSTER_GEN_BASIC] = new ZclGenBasic;
}

// can not block here for long time, for it is called in working routine
int ZigbeeDevice::queryInfo( bool queryMacAddr )
{
	Address_t		addrData;
    uint16          devAddr;
	ZclFrame		reqFrame;
	const uint16	attrID1 = ZclGenBasic::MANUNAME;
    const uint16	attrID2 = ZclGenBasic::MODELID;

	
	DBG( "ZigbeeDevice::queryInfo() %04x", addr() );
	
    devAddr = addr();
	make_address( &addrData, SHORT, &devAddr );

	// to prevent conflict with the auto reporting message.
	reqFrame.setSeqNO( SEQNO_QUERYINFO );
	reqFrame.setCmd( (uint8)ZclBase::CMD_READ_ATTR );
    
#ifdef HOST_ZCL_ENDIAN_DIFF
    BYTESWAP( &attrID1, uint16 );
    BYTESWAP( &attrID2, uint16 );
#endif
    reqFrame.appendPayload( &attrID1, 2 );
    reqFrame.appendPayload( &attrID2, 2 );


	if( queryMacAddr ) {
		return zigbee_zcl_cmd( &addrData, thisDev->endpoint(), endpoint(), profileId(),
                         ZCL_CLUSTER_ID_GEN_BASIC, &reqFrame,
						 0, NULL, NULL, parseDevQueryInfoQueryMac );
	}

	return zigbee_zcl_cmd( &addrData, thisDev->endpoint(), endpoint(), profileId(),
				 ZCL_CLUSTER_ID_GEN_BASIC, &reqFrame,
				 0, NULL, NULL, parseDevQueryInfoNoMac );
}


// called in working thread from parseUpdateDevList(), must be non-waiting
int ZigbeeDevice::queryMacAddr( )
{
	char			cmdStr[16];
	int				ret;

	DBG( "ZigbeeDevice::queryMacAddr() %04x", addr() );

	sprintf( cmdStr, "atieee %04x", addr() );
	// timeout = 0
	return zigbee_at_cmd( API_ATCMD, NULL, cmdStr, 0 );
}



int ZigbeeDevice::get_mode_id( int timeout )
{
	return clusters[CLUSTER_GEN_BASIC]->updateAttr( *this, ZclGenBasic::MODELID, timeout );
}

int ZigbeeDevice::get_manu_name( int timeout )
{
	return clusters[CLUSTER_GEN_BASIC]->updateAttr( *this, ZclGenBasic::MANUNAME, timeout );
}

int ZigbeeDevice::ping( int timeout )
{
	ZclBase *pZcl = clusters[CLUSTER_GEN_BASIC];
	return pZcl->updateAttr( *this, ZclGenBasic::ZCLVER, timeout );
}

int ZigbeeDevice::processWarning( uint8 *msg )
{
	DBG( "ZigbeeDevice::processWarning() do nothing." );
	return 0;
}


int ZigbeeDevice::update( int timeout )
{
	int ret, count = 0;

	DBG("ZigbeeDevice::update()");

//	timeout = timeout / ZIGBEE_TRYCOUNT + ( timeout % ZIGBEE_TRYCOUNT != 0 );
	
    for( ClusterList::size_type i = 1; i < clusters.size(); ++i ) {
		usleep(100*1000);
        ret = clusters[i]->updateAllAttrs( *this, timeout );
		if( ret && ETIMEDOUT == errno ) {
			if( ++count == ZIGBEE_TRYCOUNT )
				return ret;
		} else {
			count = 0;
		} // if
	} // for

	return 0;
}

// end members of ZigbeeDevice




// members of UbecPlug
UbecPlug::UbecPlug() : ZigbeeDevice(ZCL_HA_DEVICEID_UBEC_PLUG)
{
	clusters.resize(3, NULL);
	clusters[CLUSTER_ON_OFF] = new ZclGenOnOff;
	clusters[CLUSTER_POWER_METER] = new ZclUbecPowerMeter;
}

int UbecPlug::on( int timeout )
{
    int ret = ZclGenOnOff::on( *this, timeout );
	uint8 state = 1;
	ZclAttrPtr ptr;
	
    // update status
    if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr ) 			
			ptr->setData( &state, 1 );
	} // if ret

    return ret;
}


int UbecPlug::off( int timeout )
{
	int ret = ZclGenOnOff::off( *this, timeout );
	uint8 state = 0;
	ZclAttrPtr ptr;

	// update status
	if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr )			
			ptr->setData( &state, 1 );
	} // if ret

	return ret;
}  


int UbecPlug::toggle( int timeout )
{
    int ret = ZclGenOnOff::toggle( *this, timeout );
	uint8 state = 0;
	ZclAttrPtr ptr;
	
    // update status
    if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr && ptr->data() )  {
			state = *(ptr->data());		
			state = !state;
			ptr->setData( &state, 1 );
		}
	} // if ret
    
    return ret;
}

bool UbecPlug::getOnOffState() const
{
	const ZclAttrPtr ptr = clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
	if( ptr && ptr->data() )
		return *(ptr->data());
	return false;
}


float UbecPlug::getVoltage() const
{
	uint16 data;

	const ZclAttrPtr ptr = clusters[CLUSTER_POWER_METER]->getAttrLocal( ZclUbecPowerMeter::VOLTAGE );

	if( ptr && ptr->data() ) {
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 10;
	} // if
	
    return 0.0;
}


float UbecPlug::getCurrent() const
{
	uint16 data;
	const ZclAttrPtr ptr = clusters[CLUSTER_POWER_METER]->getAttrLocal( ZclUbecPowerMeter::CURRENT );

	if( ptr && ptr->data() ){
		data = to_u16 ( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 10;
	}

	return 0.0;
}

float UbecPlug::getEnergy() const
{
	uint32 data;
	ZclAttrPtr ptr = clusters[CLUSTER_POWER_METER]->getAttrLocal( ZclUbecPowerMeter::ENERGY );

	if( ptr && ptr->data() ){
		data = to_u32( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint32 );
#endif
		return (float)data / 1000;
	}

	return 0.000;
}

float UbecPlug::getPower() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_POWER_METER]->getAttrLocal( ZclUbecPowerMeter::POWER );

	if( ptr && ptr->data() ){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data /10;
	}
	return 0.0;
}

float UbecPlug::getPowerFactor() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_POWER_METER]->getAttrLocal( ZclUbecPowerMeter::POWERFACTOR );

	if( ptr && ptr->data() ){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data /100;
	}
	
	return 0.00;
}

// end members of UbecPlug





// members of UbecAirMonitor
UbecAirMonitor::UbecAirMonitor() : ZigbeeDevice(ZCL_HA_DEVICEID_UEBC_AIR_MONITOR)
{
	clusters.resize( 4, NULL );
	clusters[CLUSTER_CO2_MEASUREMENT] = new ZclUbecCO2Measurement;
//	clusters[CLUSTER_TEMP_MEASUREMENT] = new ZclUbecTempMeasurement;
	clusters[CLUSTER_TEMP_CONFIG] = new ZclGenDevTempCfg;
	clusters[CLUSTER_HUMIDITY_MEASUREMENT] = new ZclUbecHumidityMeasurement;	
}


uint16 UbecAirMonitor::getCO2Concentration() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_CO2_MEASUREMENT]->getAttrLocal( ZclUbecCO2Measurement::PPM );

	if( ptr && ptr->data()){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return data;
	}
	
	return 0;
}

float UbecAirMonitor::getTemperature() const
{
	int16 data;
//	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_MEASUREMENT]->getAttrLocal( ZclUbecTempMeasurement::MEASURED_VALUE );
	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_CONFIG]->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_CurrentTemperature );

	if( ptr && ptr->data()){
		data = to_16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, int16 );
#endif
		return (float)data / 100;
	}
	
	return 0.00;
}

float UbecAirMonitor::getMinTemperature() const
{
	int16 data;
//	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_MEASUREMENT]->getAttrLocal( ZclUbecTempMeasurement::MINIMUM_MEASURED_VALUE );
	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_CONFIG]->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_MinTempExperienced );

	if( ptr && ptr->data()){
		data = to_16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, int16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}

float UbecAirMonitor::getMaxTemperature() const
{
	int16 data;
//	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_MEASUREMENT]->getAttrLocal( ZclUbecTempMeasurement::MAXIMUM_MEASURED_VALUE );
	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_CONFIG]->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_MaxTempExperienced );

	if( ptr && ptr->data()){
		data = to_16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, int16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}

float UbecAirMonitor::getHumidity() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_HUMIDITY_MEASUREMENT]->getAttrLocal( ZclUbecHumidityMeasurement::MEASURED_VALUE );

	if( ptr && ptr->data()){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}

float UbecAirMonitor::getMinHumidity() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_HUMIDITY_MEASUREMENT]->getAttrLocal( ZclUbecHumidityMeasurement::MINIMUM_MEASURED_VALUE );

	if( ptr && ptr->data()){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}

float UbecAirMonitor::getMaxHumidity() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_HUMIDITY_MEASUREMENT]->getAttrLocal( ZclUbecHumidityMeasurement::MAXIMUM_MEASURED_VALUE );

	if( ptr && ptr->data()){
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}


float UbecAirMonitor::getLowTempThreshold() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_CONFIG]->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_LowTempThreshold );

	if( ptr && ptr->data()) {
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}


float UbecAirMonitor::getHighTempThreshold() const
{
	uint16 data;
	ZclAttrPtr ptr = clusters[CLUSTER_TEMP_CONFIG]->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_HighTempThreshold );

	if( ptr && ptr->data()) {
		data = to_u16( ptr->data() );
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &data, uint16 );
#endif
		return (float)data / 100;
	}

	return 0.00;
}


int UbecAirMonitor::setLowTempThreshold( float value, int timeout )
{
	int 		ret;
	uint16 		attrVal = (uint16)( value * 100 );
	PayloadType tmp;
	ZclPtr 		pZcl = clusters[CLUSTER_TEMP_CONFIG];
	ZclAttrPtr 	pAttr = pZcl->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_LowTempThreshold );
	
	if( !pAttr )
		return -1;

	tmp.append( (uint8*)&attrVal, 2 );
	PayloadType &attrData = pAttr->_data;
	attrData.swap( tmp );		// now tmp holds the original value of this attr

	ret = ZclBase::writeAttr( *this, pZcl->clusterID(), *pAttr, timeout );

	if( ret )
		attrData.swap( tmp );	// restore

	return ret;
}

int UbecAirMonitor::setHighTempThreshold( float value, int timeout )
{
	int 		ret;
	uint16 		attrVal = (uint16)( value * 100 );
	PayloadType tmp;
	ZclPtr 		pZcl = clusters[CLUSTER_TEMP_CONFIG];
	ZclAttrPtr 	pAttr = pZcl->getAttrLocal( ZclGenDevTempCfg::ATTR_ID_HighTempThreshold );
	
	if( !pAttr )
		return -1;

	tmp.append( (uint8*)&attrVal, 2 );
	PayloadType &attrData = pAttr->_data;
	attrData.swap( tmp );		// now tmp holds the original value of this attr

	ret = ZclBase::writeAttr( *this, pZcl->clusterID(), *pAttr, timeout );

	if( ret )
		attrData.swap( tmp );	// restore

	return ret;
}



// running through ThreadSafePtrList::find_op(), list is on lock
int UbecAirMonitor::processWarning( uint8 *msg )
{
	uint8			alarmCode;
	uint16			clusterID;
	int				ret;

	DBG( "UbecAirMonitor::processWarning" );
	
	if( !msg )
		return 1;

	alarmCode = *msg;
	clusterID = to_u16( msg + 1 );
#ifdef HOST_ZCL_ENDIAN_DIFF
	BYTESWAP( &clusterID, uint16 );
#endif
	warningCode = (int)clusterID << 16 | (uint16)alarmCode;
	
    if( eventHandler )
        ret = eventHandler( ZIGBEE_WARNING, this );
	
	return ret;
}


// end members of UbecAirMonitor


// members of UbecSimplePlug
UbecSimplePlug::UbecSimplePlug() : ZigbeeDevice(ZCL_HA_DEVICEID_UEBC_SIMPLE_PLUG)
{
	clusters.resize(2, NULL);
	clusters[CLUSTER_ON_OFF] = new ZclGenOnOff;
}


//void UbecSimplePlug::update( int timeout )
//{    
//    DBG( "UbecSimplePlug::update()" );
//    // ZCL_GEN_BASIC not included
//    for( ClusterList::size_type i = 1; i < clusters.size(); ++i )
//        clusters[i]->getAttr( *this, timeout );
//}

int UbecSimplePlug::on( int timeout )
{
    int ret = ZclGenOnOff::on( *this, timeout );
	uint8 state = 1;
	ZclAttrPtr ptr;
	
    // update status
    if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr ) 			
			ptr->setData( &state, 1 );
	} // if ret

    return ret;
}


int UbecSimplePlug::off( int timeout )
{
	int ret = ZclGenOnOff::off( *this, timeout );
	uint8 state = 0;
	ZclAttrPtr ptr;

	// update status
	if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr )			
			ptr->setData( &state, 1 );
	} // if ret

	return ret;
}  
	

int UbecSimplePlug::toggle( int timeout )
{
    int ret = ZclGenOnOff::toggle( *this, timeout );
	uint8 state = 0;
	ZclAttrPtr ptr;
	
    // update status
    if( 0 == ret ) {
		ptr = (ZclAttrPtr)clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
		if( ptr && ptr->data() )  {
			state = *(ptr->data());		
			state = !state;
			ptr->setData( &state, 1 );
		}
	} // if ret
    
    return ret;
}

bool UbecSimplePlug::getOnOffState() const
{
	const ZclAttrPtr ptr = clusters[CLUSTER_ON_OFF]->getAttrLocal( ZclGenOnOff::ON_OFF );
	if( ptr && ptr->data() )
		return *(ptr->data());
	return false;
}

// end members of UbecSimplePlug


