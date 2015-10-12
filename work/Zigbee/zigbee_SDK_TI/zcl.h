#ifndef _ZCL_H
#define _ZCL_H

#include "zigbee_common.h"
//#include "zdev.h"

#define SEC_KEY_LEN  16  // 128/8 octets (128-bit key is standard for ZigBee)

#define BREAK_UINT32( var, ByteNum ) \
(uint8)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
((uint32)((uint32)((Byte0) & 0x00FF) \
          + ((uint32)((Byte1) & 0x00FF) << 8) \
          + ((uint32)((Byte2) & 0x00FF) << 16) \
          + ((uint32)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)


#define ZCL_HA_PROFILE_ID             	0x0104
// devices
// Generic Device IDs
#define ZCL_HA_DEVICEID_ON_OFF_SWITCH                           0x0000
#define ZCL_HA_DEVICEID_LEVEL_CONTROL_SWITCH                    0x0001
#define ZCL_HA_DEVICEID_ON_OFF_OUTPUT                           0x0002
#define ZCL_HA_DEVICEID_LEVEL_CONTROLLABLE_OUTPUT               0x0003
#define ZCL_HA_DEVICEID_SCENE_SELECTOR                          0x0004
#define ZCL_HA_DEVICEID_CONFIGURATIOPN_TOOL                     0x0005
#define ZCL_HA_DEVICEID_REMOTE_CONTROL                          0x0006
#define ZCL_HA_DEVICEID_COMBINED_INETRFACE                      0x0007
#define ZCL_HA_DEVICEID_RANGE_EXTENDER                          0x0008
#define ZCL_HA_DEVICEID_MAINS_POWER_OUTLET                      0x0009
  // temp: nnl
#define ZCL_HA_DEVICEID_TEST_DEVICE                             0x00FF

// Lighting Device IDs
#define ZCL_HA_DEVICEID_ON_OFF_LIGHT                            0x0100
#define ZCL_HA_DEVICEID_DIMMABLE_LIGHT                          0x0101
#define ZCL_HA_DEVICEID_COLORED_DIMMABLE_LIGHT                  0x0102
#define ZCL_HA_DEVICEID_ON_OFF_LIGHT_SWITCH                     0x0103
#define ZCL_HA_DEVICEID_DIMMER_SWITCH                           0x0104
#define ZCL_HA_DEVICEID_COLOR_DIMMER_SWITCH                     0x0105
#define ZCL_HA_DEVICEID_LIGHT_SENSOR                            0x0106
#define ZCL_HA_DEVICEID_OCCUPANCY_SENSOR                        0x0107

// Closures Device IDs
#define ZCL_HA_DEVICEID_SHADE                                   0x0200
#define ZCL_HA_DEVICEID_SHADE_CONTROLLER                        0x0201

// HVAC Device IDs
#define ZCL_HA_DEVICEID_HEATING_COOLING_UNIT                    0x0300
#define ZCL_HA_DEVICEID_THERMOSTAT                              0x0301
#define ZCL_HA_DEVICEID_TEMPERATURE_SENSOR                      0x0302
#define ZCL_HA_DEVICEID_PUMP                                    0x0303
#define ZCL_HA_DEVICEID_PUMP_CONTROLLER                         0x0304
#define ZCL_HA_DEVICEID_PRESSURE_SENSOR                         0x0305
#define ZCL_HA_DEVICEID_FLOW_SENSOR                             0x0306

// Intruder Alarm Systems (IAS) Device IDs
#define ZCL_HA_DEVICEID_IAS_CONTROL_INDICATING_EQUIPMENT        0x0400
#define ZCL_HA_DEVICEID_IAS_ANCILLARY_CONTROL_EQUIPMENT         0x0401
#define ZCL_HA_DEVICEID_IAS_ZONE                                0x0402
#define ZCL_HA_DEVICEID_IAS_WARNING_DEVICE                      0x0403




// cluster ids
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG              0x0007


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


/*** Data Types ***/
#define ZCL_DATATYPE_NO_DATA                            0x00
#define ZCL_DATATYPE_DATA8                              0x08
#define ZCL_DATATYPE_DATA16                             0x09
#define ZCL_DATATYPE_DATA24                             0x0a
#define ZCL_DATATYPE_DATA32                             0x0b
#define ZCL_DATATYPE_DATA40                             0x0c
#define ZCL_DATATYPE_DATA48                             0x0d
#define ZCL_DATATYPE_DATA56                             0x0e
#define ZCL_DATATYPE_DATA64                             0x0f
#define ZCL_DATATYPE_BOOLEAN                            0x10
#define ZCL_DATATYPE_BITMAP8                            0x18
#define ZCL_DATATYPE_BITMAP16                           0x19
#define ZCL_DATATYPE_BITMAP24                           0x1a
#define ZCL_DATATYPE_BITMAP32                           0x1b
#define ZCL_DATATYPE_BITMAP40                           0x1c
#define ZCL_DATATYPE_BITMAP48                           0x1d
#define ZCL_DATATYPE_BITMAP56                           0x1e
#define ZCL_DATATYPE_BITMAP64                           0x1f
#define ZCL_DATATYPE_UINT8                              0x20
#define ZCL_DATATYPE_UINT16                             0x21
#define ZCL_DATATYPE_UINT24                             0x22
#define ZCL_DATATYPE_UINT32                             0x23
#define ZCL_DATATYPE_UINT40                             0x24
#define ZCL_DATATYPE_UINT48                             0x25
#define ZCL_DATATYPE_UINT56                             0x26
#define ZCL_DATATYPE_UINT64                             0x27
#define ZCL_DATATYPE_INT8                               0x28
#define ZCL_DATATYPE_INT16                              0x29
#define ZCL_DATATYPE_INT24                              0x2a
#define ZCL_DATATYPE_INT32                              0x2b
#define ZCL_DATATYPE_INT40                              0x2c
#define ZCL_DATATYPE_INT48                              0x2d
#define ZCL_DATATYPE_INT56                              0x2e
#define ZCL_DATATYPE_INT64                              0x2f
#define ZCL_DATATYPE_ENUM8                              0x30
#define ZCL_DATATYPE_ENUM16                             0x31
#define ZCL_DATATYPE_SEMI_PREC                          0x38
#define ZCL_DATATYPE_SINGLE_PREC                        0x39
#define ZCL_DATATYPE_DOUBLE_PREC                        0x3a
#define ZCL_DATATYPE_OCTET_STR                          0x41
#define ZCL_DATATYPE_CHAR_STR                           0x42
#define ZCL_DATATYPE_LONG_OCTET_STR                     0x43
#define ZCL_DATATYPE_LONG_CHAR_STR                      0x44
#define ZCL_DATATYPE_ARRAY                              0x48
#define ZCL_DATATYPE_STRUCT                             0x4c
#define ZCL_DATATYPE_SET                                0x50
#define ZCL_DATATYPE_BAG                                0x51
#define ZCL_DATATYPE_TOD                                0xe0
#define ZCL_DATATYPE_DATE                               0xe1
#define ZCL_DATATYPE_UTC                                0xe2
#define ZCL_DATATYPE_CLUSTER_ID                         0xe8
#define ZCL_DATATYPE_ATTR_ID                            0xe9
#define ZCL_DATATYPE_BAC_OID                            0xea
#define ZCL_DATATYPE_IEEE_ADDR                          0xf0
#define ZCL_DATATYPE_128_BIT_SEC_KEY                    0xf1
#define ZCL_DATATYPE_UNKNOWN                            0xff

/*** Error Status Codes ***/
#define ZCL_STATUS_SUCCESS                              0x00
#define ZCL_STATUS_FAILURE                              0x01
// 0x02-0x7D are reserved.
#define ZCL_STATUS_NOT_AUTHORIZED                       0x7E
#define ZCL_STATUS_MALFORMED_COMMAND                    0x80
#define ZCL_STATUS_UNSUP_CLUSTER_COMMAND                0x81
#define ZCL_STATUS_UNSUP_GENERAL_COMMAND                0x82
#define ZCL_STATUS_UNSUP_MANU_CLUSTER_COMMAND           0x83
#define ZCL_STATUS_UNSUP_MANU_GENERAL_COMMAND           0x84
#define ZCL_STATUS_INVALID_FIELD                        0x85
#define ZCL_STATUS_UNSUPPORTED_ATTRIBUTE                0x86
#define ZCL_STATUS_INVALID_VALUE                        0x87
#define ZCL_STATUS_READ_ONLY                            0x88
#define ZCL_STATUS_INSUFFICIENT_SPACE                   0x89
#define ZCL_STATUS_DUPLICATE_EXISTS                     0x8a
#define ZCL_STATUS_NOT_FOUND                            0x8b
#define ZCL_STATUS_UNREPORTABLE_ATTRIBUTE               0x8c
#define ZCL_STATUS_INVALID_DATA_TYPE                    0x8d
#define ZCL_STATUS_INVALID_SELECTOR                     0x8e
#define ZCL_STATUS_WRITE_ONLY                           0x8f
#define ZCL_STATUS_INCONSISTENT_STARTUP_STATE           0x90
#define ZCL_STATUS_DEFINED_OUT_OF_BAND                  0x91
#define ZCL_STATUS_INCONSISTENT                         0x92
#define ZCL_STATUS_ACTION_DENIED                        0x93
#define ZCL_STATUS_TIMEOUT                              0x94
#define ZCL_STATUS_ABORT                                0x95
#define ZCL_STATUS_INVALID_IMAGE                        0x96
#define ZCL_STATUS_WAIT_FOR_DATA                        0x97
#define ZCL_STATUS_NO_IMAGE_AVAILABLE                   0x98
#define ZCL_STATUS_REQUIRE_MORE_IMAGE                   0x99

// 0xbd-bf are reserved.
#define ZCL_STATUS_HARDWARE_FAILURE                     0xc0
#define ZCL_STATUS_SOFTWARE_FAILURE                     0xc1
#define ZCL_STATUS_CALIBRATION_ERROR                    0xc2


#define ZCL_DATA_VAR_LEN								-1


class ZclFrame : public ZigbeePayload {
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
    
    void setProfile( uint16 profileid )
    { _profileID = profileid; }
    uint16 profileID() const
    { return _profileID; }
    
    void setCluster( uint16 clusterid )
    { _clusterID = clusterid; }
    uint16 clusterID() const
    { return _clusterID; }
    
    void setSrcEp( uint8 srcep )
    { _srcEp = srcep; }
    uint8 srcEp() const
    { return _srcEp; }
    
    void setDstEp( uint8 dstep )
    { _dstEp = dstep; }
    uint8 dstEp() const
    { return _dstEp; }
    
    void setAddr( const ZigbeeAddr &address )
    { _addr = address; }
    const ZigbeeAddr& addr() const
    { return _addr; }
protected:
    uint16          _profileID;
    uint16          _clusterID;
    uint8           _srcEp;
    uint8           _dstEp;
    ZigbeeAddr      _addr;
    uint8			_frameCtrl;
    uint16          _manufacturerCode;
    uint8           _seqNO;
    uint8           _cmdId;

#ifdef _DEBUG
public:
	const char *toString() const
	{
		static char 			buf[1024];
		char					*ptr = buf;
		uint16					i;

		ptr += sprintf( ptr, "ZCL Frame seqNO:\t\t\t%02x\n", seqNO() );
		ptr += sprintf( ptr, "From:\t\t\t%s:%02x->%02x\n", addr().toString(), srcEp(), dstEp() );
		ptr += sprintf( ptr, "Profile:\t\t\t%04x\n", profileID() );
		ptr += sprintf( ptr, "Cluster:\t\t\t%04x\n", clusterID() );
		ptr += sprintf( ptr, "FrameCtrl:\t\t\t%02x\n", _frameCtrl );
		ptr += sprintf( ptr, "FrameCtrl:\t\t\t%s %s %s %s\n", isClusterSpec() ? "ClusterSpec" : "",
			   isManuSpec() ? "ManuSpec" : "", isFromServer() ? "FromServer" : "",
			   responseDisabled() ? "responseDisabled" : "" );
		if( isManuSpec() )
			ptr += sprintf( ptr, "ManuCode:\t\t\t%04x\n", manuCode() );
		ptr += sprintf( ptr, "CmdID:\t\t\t%02x\n", cmd() );
		ptr += sprintf( ptr, "dataLen:\t\t\t%u\n", dataLen() );
		ptr += sprintf( ptr, "data:\t\t\t" );
		pr_mem_str( data(), dataLen(), ptr );
		sprintf( ptr, "\n" );

		return buf;
	}
#endif
};

typedef int (*ZclCmdFunc)( ZigbeeDevice &dev, int timeout );


class ZclAttr;
typedef ZclAttr* ZclAttrPtr;
typedef std::vector<ZclAttr> ZclAttrList;



class ZclBase {
public:
	enum { CMD_READ_ATTR = 0x0, CB_READ_ATTR = 0x01, 
			CMD_WRITE_ATTR = 0x02, CB_WRITE_ATTR = 0x04, CMD_DISCOVER_ATTR = 0x0C };
public:
	virtual uint16 clusterID() const = 0;
	virtual ZclAttrPtr getAttrList() = 0;
	virtual uint16 getAttrListLen() const = 0;

	int updateAttr( ZigbeeDevice &dev, uint16 attrID, int timeout );	
	int updateAllAttrs( ZigbeeDevice &dev, int timeout );
    ZclAttrPtr getAttrLocal( uint16 attrID );
    void  setAttrLocal( uint16 attrID, const void *buf, uint16 len );

	virtual ~ZclBase() {}
public:
	static int zclDataTypeLen(uint8 dataType);
	static ZclBase* getClusterByID( uint16 clusterID );
	
	static int readAttr( ZigbeeDevice &dev, uint16 clusterId, ZclAttr &attr, int timeout );

	//!! in testing
	static int readAttrGroup( ZigbeeDevice &dev, uint16 clusterId, 
					ZclAttr *attrArray, uint16 arrayLen, int timeout );

	static int writeAttr( ZigbeeDevice &dev, uint16 clusterId, ZclAttr &attr, int timeout );

	// return 0 on success -1 on system error, n number of attrs failed to write.
	static int writeAttrGroup( ZigbeeDevice &dev, uint16 clusterId, 
					ZclAttr *attrArray, uint16 arrayLen, int timeout );
    
	// for those req zcl frames only containing cmd ID
	// and rsp zcl frame only containing cmdid and status
	static int issueZclCmd( ZigbeeDevice &dev, uint16 clusterID, uint8 cmdID, 
						void *arg, uint16 arglen, int timeout );

};

typedef ZclBase* ZclPtr;

class ZclCompare : public ZclBase {
public:
	ZclCompare() {}

	// for finding in set
	ZclCompare( uint16 cid )
	{ _clusterID = cid; }

	// for finding in set
	operator ZclPtr()
	{ return this; }
	
	bool operator() ( const ZclPtr pLeft, const ZclPtr pRight ) const
	{ return pLeft->clusterID() < pRight->clusterID(); }

	virtual ZclAttrPtr getAttrList()
	{ return NULL; }

	virtual uint16 getAttrListLen() const
	{ return 0; }

	virtual uint16 clusterID() const
	{ return _clusterID; }
private:
	uint16			_clusterID;
};




struct ZclAttr : public ZigbeePayload {
	typedef enum { RDONLY = 1, WRONLY, RDWR } AccessType;
	
	ZclAttr() {}
	ZclAttr( uint16 _id, const char *_name, uint8 type, AccessType ac, const void *defaultVal = NULL )
			    : id(_id), name(_name), dataType(type), accessCtrl(ac),
			    status(ZCL_STATUS_WAIT_FOR_DATA)
	{
		if( defaultVal )
			this->setData( defaultVal, ZclBase::zclDataTypeLen(dataType) );
	}

	bool isReadable() const
	{ return accessCtrl & RDONLY; }
	bool isWritable() const
	{ return accessCtrl & WRONLY; }
	
	
	uint16			id;
	const char		*name;
	uint8			dataType;
	uint8			status;
	uint8			accessCtrl;
	
#ifdef _DEBUG
	char *toString()
	{
		static char				buf[256];
		char					*ptr = buf;

		ptr += sprintf( ptr, "ZCL Attribute: %04x, %s\n", id, name );
		ptr += sprintf( ptr, "DataType:\t\t\t%02x\n", dataType );
		ptr += sprintf( ptr, "status:\t\t\t%02x\n", status );
		if( 0 == status ) {
			ptr += sprintf( ptr, "dataLen:\t\t\t%u\n", dataLen() );
			ptr += sprintf( ptr, "data:\t\t\t" );
			pr_mem_str( data(), dataLen(), ptr );
		} // if
		sprintf( ptr, "\n" );

		return buf;
	}
#endif 
};


//!! constructors must not appeared, for initialize array cmds in each cluster in zcl.cpp
struct ZclCmd {
//	ZclCmd() {}
//	ZclCmd( uint8 _id, const char *_name, ZclCmdFunc _func )
//		: id(_id), name(_name), func(_func) {}
	
	uint8				id;
	const char			*name;
	ZclCmdFunc			func;
};
typedef std::vector<ZclCmd> ZclCmdList;


template <typename T>
struct ZclElemFinder {
	typedef enum { BYID = 0x01, BYNAME = 0x01 << 1 } MethodType;

	ZclElemFinder( MethodType _method, uint16 _id, const char *_name = NULL ) 
		: method(_method), id(_id), name(_name) {}
	
	bool operator() ( const T &data ) const
	{
		if( (method & BYID) && id != data.id )
			return false;
		if( (method & BYNAME) && strcmp(name, data.name) )
			return false;
		return true;
	}

	uint16				id;
	const char			*name;
	MethodType			method;
};



class ZclGenBasic : public ZclBase {
	static const uint16 		thisClusterID = ZCL_CLUSTER_ID_GEN_BASIC;
public:
	static const uint16				N_ATTR = 8;
public:
	enum { ZCLVER, APPVER, STACKVER, HWVER, MANUNAME, MODELID, DATECODE, POWERSRC };
public:
	ZclGenBasic()
	{
		attrs[ZCLVER] = ZclAttr( ZCLVER, "ZCLVersion", ZCL_DATATYPE_UINT8, ZclAttr::RDONLY );
		attrs[APPVER] = ZclAttr( APPVER, "ApplicationVersion", ZCL_DATATYPE_UINT8, ZclAttr::RDONLY  );
		attrs[STACKVER] = ZclAttr( STACKVER, "StackVersion", ZCL_DATATYPE_UINT8, ZclAttr::RDONLY  );
		attrs[HWVER] = ZclAttr( HWVER, "HWVersion", ZCL_DATATYPE_UINT8, ZclAttr::RDONLY  );
		attrs[MANUNAME] = ZclAttr( MANUNAME, "ManufacturerName", ZCL_DATATYPE_CHAR_STR, ZclAttr::RDONLY  );
		attrs[MODELID] = ZclAttr( MODELID, "ModelIdentifier", ZCL_DATATYPE_CHAR_STR, ZclAttr::RDONLY  );
		attrs[DATECODE] = ZclAttr( DATECODE, "DateCode", ZCL_DATATYPE_CHAR_STR, ZclAttr::RDONLY  );
		attrs[POWERSRC] = ZclAttr( POWERSRC, "PowerSource", ZCL_DATATYPE_ENUM8, ZclAttr::RDONLY  );
	}

	virtual ZclAttrPtr getAttrList()
	{ return attrs; }

	virtual uint16 getAttrListLen() const
	{ return N_ATTR; }
	
	virtual uint16 clusterID() const
	{ return ZCL_CLUSTER_ID_GEN_BASIC; }
protected:
	ZclAttr 		attrs[N_ATTR];
};



//class ZclGenPowerCfg : public ZclBase {
//	static const uint16 		thisClusterID = ZCL_CLUSTER_ID_GEN_POWER_CFG;
//	static const uint16			N_ATTR = 12;
//public:
//	enum { ATTR_ID_MainsVoltage,
//			ATTR_ID_MainsFrequency,
//			ATTR_ID_MainsVoltageMinThreshold = 0x0011,
//			ATTR_ID_MainsVoltageMaxThreshold,
//			ATTR_ID_MainsVoltageDwellTripPoint,
//			ATTR_ID_BatteryVoltage = 0x0020,
//			ATTR_ID_BatteryManufacturer = 0x0030,
//			ATTR_ID_BatterySize,
//			ATTR_ID_BatteryAHrRating,
//			ATTR_ID_BatteryQuantity,
//			ATTR_ID_BatteryRatedVoltage,
//			ATTR_ID_BatteryVoltageMinThreshold = 0x0036 };
//public:
//	enum { ATTR_IDX_MainsVoltage,
//			ATTR_IDX_MainsFrequency,
//			ATTR_IDX_MainsVoltageMinThreshold,
//			ATTR_IDX_MainsVoltageMaxThreshold,
//			ATTR_IDX_MainsVoltageDwellTripPoint,
//			ATTR_IDX_BatteryVoltage,
//			ATTR_IDX_BatteryManufacturer,
//			ATTR_IDX_BatterySize,
//			ATTR_IDX_BatteryAHrRating,
//			ATTR_IDX_BatteryQuantity,
//			ATTR_IDX_BatteryRatedVoltage,
//			ATTR_IDX_BatteryVoltageMinThreshold };
//public:
//	ZclGenPowerCfg()
//	{
//		attrs[ATTR_IDX_MainsVoltage] = ZclAttr( (uint16)ATTR_ID_MainsVoltage, "MainsVoltage", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_MainsFrequency] = ZclAttr( (uint16)ATTR_ID_MainsFrequency, "MainsFrequency", ZCL_DATATYPE_UINT8 );
//		attrs[ATTR_IDX_MainsVoltageMinThreshold] = ZclAttr( (uint16)ATTR_ID_MainsVoltageMinThreshold, "MainsVoltageMinThreshold", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_MainsVoltageMaxThreshold] = ZclAttr( (uint16)ATTR_ID_MainsVoltageMaxThreshold, "MainsVoltageMaxThreshold", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_MainsVoltageDwellTripPoint] = ZclAttr( (uint16)ATTR_ID_MainsVoltageDwellTripPoint, "MainsVoltageDwellTripPoint", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_BatteryVoltage] = ZclAttr( (uint16)ATTR_ID_BatteryVoltage, "BatteryVoltage", ZCL_DATATYPE_UINT8 );
//		attrs[ATTR_IDX_BatteryManufacturer] = ZclAttr( (uint16)ATTR_ID_BatteryManufacturer, "BatteryManufacturer", ZCL_DATATYPE_CHAR_STR );
//		attrs[ATTR_IDX_BatterySize] = ZclAttr( (uint16)ATTR_ID_BatterySize, "BatterySize", ZCL_DATATYPE_ENUM8 );
//		attrs[ATTR_IDX_BatteryAHrRating] = ZclAttr( (uint16)ATTR_ID_BatteryAHrRating, "BatteryAHrRating", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_BatteryQuantity] = ZclAttr( (uint16)ATTR_ID_BatteryQuantity, "BatteryQuantity", ZCL_DATATYPE_UINT8 );
//		attrs[ATTR_IDX_BatteryRatedVoltage] = ZclAttr( (uint16)ATTR_ID_BatteryRatedVoltage, "BatteryRatedVoltage", ZCL_DATATYPE_UINT8 );
//		attrs[ATTR_IDX_BatteryVoltageMinThreshold] = ZclAttr( (uint16)ATTR_ID_BatteryVoltageMinThreshold, "BatteryVoltageMinThreshold", ZCL_DATATYPE_UINT8 );
//	}
//	
//	virtual ZclAttrPtr getAttrList()
//	{ return attrs; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//		
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttr 		attrs[N_ATTR];
//};


//class ZclGenDevTempCfg : public ZclBase {
//	static const uint16 		thisClusterID = ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG;
//	static const uint16			N_ATTR = 8;
//public:		// attr ids
//	enum { ATTR_ID_CurrentTemperature,
//			ATTR_ID_MinTempExperienced,
//			ATTR_ID_MaxTempExperienced,
//			ATTR_ID_OverTempTotalDwell,
//			ATTR_ID_LowTempThreshold = 0x0011,
//			ATTR_ID_HighTempThreshold,
//			ATTR_ID_LowTempDwellTripPoint,
//			ATTR_ID_HighTempDwellTripPoint };
//public:		// attr index
//	enum { ATTR_IDX_CurrentTemperature,
//			ATTR_IDX_MinTempExperienced,
//			ATTR_IDX_MaxTempExperienced,
//			ATTR_IDX_OverTempTotalDwell,
//			ATTR_IDX_LowTempThreshold,
//			ATTR_IDX_HighTempThreshold,
//			ATTR_IDX_LowTempDwellTripPoint,
//			ATTR_IDX_HighTempDwellTripPoint };
//public:
//	ZclGenDevTempCfg()
//	{
//		attrs[ATTR_IDX_CurrentTemperature] = ZclAttr( (uint16)ATTR_ID_CurrentTemperature, "CurrentTemperature", ZCL_DATATYPE_DATA16 );
//		attrs[ATTR_IDX_MinTempExperienced] = ZclAttr( (uint16)ATTR_ID_MinTempExperienced, "MinTempExperienced", ZCL_DATATYPE_DATA16 );
//		attrs[ATTR_IDX_MaxTempExperienced] = ZclAttr( (uint16)ATTR_ID_MaxTempExperienced, "MaxTempExperienced", ZCL_DATATYPE_DATA16 );
//		attrs[ATTR_IDX_OverTempTotalDwell] = ZclAttr( (uint16)ATTR_ID_OverTempTotalDwell, "OverTempTotalDwell", ZCL_DATATYPE_UINT16 );
//		attrs[ATTR_IDX_LowTempThreshold] = ZclAttr( (uint16)ATTR_ID_LowTempThreshold, "LowTempThreshold", ZCL_DATATYPE_DATA16 );
//		attrs[ATTR_IDX_HighTempThreshold] = ZclAttr( (uint16)ATTR_ID_HighTempThreshold, "HighTempThreshold", ZCL_DATATYPE_DATA16 );
//		attrs[ATTR_IDX_LowTempDwellTripPoint] = ZclAttr( (uint16)ATTR_ID_LowTempDwellTripPoint, "LowTempDwellTripPoint", ZCL_DATATYPE_UINT24 );
//		attrs[ATTR_IDX_HighTempDwellTripPoint] = ZclAttr( (uint16)ATTR_ID_HighTempDwellTripPoint, "HighTempDwellTripPoint", ZCL_DATATYPE_UINT24 );
//	}
//	
//	virtual ZclAttrPtr getAttrList()
//	{ return attrs; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//    	
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttr			attrs[N_ATTR];
//};



class ZclGenOnOff : public ZclBase {
	static const uint16 		thisClusterID = ZCL_CLUSTER_ID_GEN_ON_OFF;
public:
	static const uint16			N_ATTR = 1;
	static const uint8			N_CMD = 3;
//	static const ZclCmd			cmds[N_CMD];
public:
	enum { ON_OFF };
	enum { OFF, ON, TOGGLE };
public:
	ZclGenOnOff()
	{
		attrs[ON_OFF] = ZclAttr( (uint16)ON_OFF, "OnOff", ZCL_DATATYPE_BOOLEAN, ZclAttr::RDONLY );
	}
	
	static int on( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, thisClusterID, (uint8)ON, NULL, 0, timeout );
	}
	
	static int off( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, thisClusterID, (uint8)OFF, NULL, 0, timeout );
	}
    
	static int toggle( ZigbeeDevice &dev, int timeout )
	{
		return issueZclCmd( dev, thisClusterID, (uint8)TOGGLE, NULL, 0, timeout );
	}

	virtual ZclAttrPtr getAttrList()
	{ return attrs; }

	virtual uint16 getAttrListLen() const
	{ return N_ATTR; }
    	
	virtual uint16 clusterID() const 
	{ return thisClusterID; }
protected:
	ZclAttr			attrs[N_ATTR];
};





//class ZclUbecPowerMeter : public ZclBase {
//	static const uint16			thisClusterID = ZCL_CLUSTER_ID_UBEC_POWERMETER;
//public:
//	static const uint16 		N_ATTR = 5;
//public:
//	enum { VOLTAGE = 0x0, CURRENT = 0x0010, POWER = 0x0020, ENERGY = 0x0030,
//        POWERFACTOR = 0x0040 };
//public:
//	ZclUbecPowerMeter()
//	{
//		attrs.push_back( ZclAttr((uint16)VOLTAGE, "Voltage", ZCL_DATATYPE_UINT16) );
//		attrs.push_back( ZclAttr((uint16)CURRENT, "Current", ZCL_DATATYPE_UINT16) );
//		attrs.push_back( ZclAttr((uint16)POWER, "Power", ZCL_DATATYPE_UINT16) );
//		attrs.push_back( ZclAttr((uint16)ENERGY, "Energy", ZCL_DATATYPE_UINT32) );
//		attrs.push_back( ZclAttr((uint16)POWERFACTOR, "PowerFactor", ZCL_DATATYPE_UINT16) );
//	}

//	virtual ZclAttrPtr getAttrList()
//	{ return &attrs[0]; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//    
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttrList			attrs;
//};



//class ZclUbecCO2Measurement : public ZclBase {
//	static const uint16			thisClusterID = ZCL_CLUSTER_ID_UBEC_CO2_MEASUREMENT;
//public:
//	static const uint16 		N_ATTR = 1;
//public:
//	enum { PPM = 0x0 };
//public:
//	ZclUbecCO2Measurement()
//	{
//		attrs.push_back( ZclAttr((uint16)PPM, "PPM", ZCL_DATATYPE_UINT16) );
//	}

//	virtual ZclAttrPtr getAttrList()
//	{ return &attrs[0]; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//	
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttrList			attrs;
//};

//class ZclUbecTempMeasurement : public ZclBase {
//	static const uint16			thisClusterID = ZCL_CLUSTER_ID_UBEC_TEMP_MEASUREMENT;
//public:
//	static const uint16 		N_ATTR = 3;
//public:
//	enum { MEASURED_VALUE = 0x0,
//		     MINIMUM_MEASURED_VALUE = 0x0001,
//		     MAXIMUM_MEASURED_VALUE = 0x0002};
//public:
//	ZclUbecTempMeasurement()
//	{
//		attrs.push_back( ZclAttr((uint16)MEASURED_VALUE, "MeasuredValue", ZCL_DATATYPE_INT16) );
//		attrs.push_back( ZclAttr((uint16)MINIMUM_MEASURED_VALUE, "MinimumMeasuredValue", ZCL_DATATYPE_INT16) );
//		attrs.push_back( ZclAttr((uint16)MAXIMUM_MEASURED_VALUE, "MaximumMeasuredValue", ZCL_DATATYPE_INT16) );
//	}

//	virtual ZclAttrPtr getAttrList()
//	{ return &attrs[0]; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//    
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttrList			attrs;
//};

//class ZclUbecHumidityMeasurement : public ZclBase {
//	static const uint16			thisClusterID = ZCL_CLUSTER_ID_UBEC_HUMIDITY_MEASUREMENT;
//public:
//	static const uint16 		N_ATTR = 3;
//public:
//	enum { MEASURED_VALUE = 0x0,
//		     MINIMUM_MEASURED_VALUE = 0x0001,
//		     MAXIMUM_MEASURED_VALUE = 0x0002};
//public:
//	ZclUbecHumidityMeasurement()
//	{
//		attrs.push_back( ZclAttr((uint16)MEASURED_VALUE, "MeasuredValue", ZCL_DATATYPE_UINT16) );
//		attrs.push_back( ZclAttr((uint16)MINIMUM_MEASURED_VALUE, "MinimumMeasuredValue", ZCL_DATATYPE_UINT16) );
//		attrs.push_back( ZclAttr((uint16)MAXIMUM_MEASURED_VALUE, "MaximumMeasuredValue", ZCL_DATATYPE_UINT16) );
//	}

//	virtual ZclAttrPtr getAttrList()
//	{ return &attrs[0]; }

//	virtual uint16 getAttrListLen() const
//	{ return N_ATTR; }
//	
//	virtual uint16 clusterID() const 
//	{ return thisClusterID; }
//protected:
//	ZclAttrList			attrs;
//};



#endif
