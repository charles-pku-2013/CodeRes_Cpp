#include "zcl.h"
#include "zdev.h"



int ZclBase::zclDataTypeLen(uint8 dataType)
{
	switch(dataType)
	{
	case ZCL_DATATYPE_DATA8:
	case ZCL_DATATYPE_BOOLEAN:
	case ZCL_DATATYPE_BITMAP8:
	case ZCL_DATATYPE_INT8:
	case ZCL_DATATYPE_UINT8:
	case ZCL_DATATYPE_ENUM8:
		return 1;
	case ZCL_DATATYPE_DATA16:
	case ZCL_DATATYPE_BITMAP16:
	case ZCL_DATATYPE_UINT16:
	case ZCL_DATATYPE_INT16:
	case ZCL_DATATYPE_ENUM16:
	case ZCL_DATATYPE_SEMI_PREC:
	case ZCL_DATATYPE_CLUSTER_ID:
	case ZCL_DATATYPE_ATTR_ID:
		return 2;
	case ZCL_DATATYPE_DATA24:
	case ZCL_DATATYPE_BITMAP24:
	case ZCL_DATATYPE_UINT24:
	case ZCL_DATATYPE_INT24:
		return 3;
	case ZCL_DATATYPE_DATA32:
	case ZCL_DATATYPE_BITMAP32:
	case ZCL_DATATYPE_UINT32:
	case ZCL_DATATYPE_INT32:
	case ZCL_DATATYPE_SINGLE_PREC:
	case ZCL_DATATYPE_TOD:
	case ZCL_DATATYPE_DATE:
	case ZCL_DATATYPE_UTC:
	case ZCL_DATATYPE_BAC_OID:
		return 4;
	case ZCL_DATATYPE_DATA40:
	case ZCL_DATATYPE_BITMAP40:
	case ZCL_DATATYPE_UINT40:
	case ZCL_DATATYPE_INT40:
		return 5;
	case ZCL_DATATYPE_DATA48:
	case ZCL_DATATYPE_BITMAP48:
	case ZCL_DATATYPE_UINT48:
	case ZCL_DATATYPE_INT48:
		return 6;
	case ZCL_DATATYPE_DATA56:
	case ZCL_DATATYPE_BITMAP56:
	case ZCL_DATATYPE_UINT56:
	case ZCL_DATATYPE_INT56:
		return 7;
	case ZCL_DATATYPE_DATA64:
	case ZCL_DATATYPE_BITMAP64:
	case ZCL_DATATYPE_UINT64:
	case ZCL_DATATYPE_INT64:
	case ZCL_DATATYPE_DOUBLE_PREC:
	case ZCL_DATATYPE_IEEE_ADDR:
		return 8;
	case ZCL_DATATYPE_128_BIT_SEC_KEY:
		return 16;
	case ZCL_DATATYPE_CHAR_STR:
	case ZCL_DATATYPE_OCTET_STR:
	case ZCL_DATATYPE_LONG_CHAR_STR:
	case ZCL_DATATYPE_LONG_OCTET_STR:
	case ZCL_DATATYPE_ARRAY:
	case ZCL_DATATYPE_STRUCT:
	case ZCL_DATATYPE_SET:
	case ZCL_DATATYPE_BAG:
		return ZCL_DATA_VAR_LEN;
	}

	return 0;
}




ZclPtr ZclBase::getClusterByID( uint16 clusterID )
{
	switch( clusterID ) {
	case ZCL_CLUSTER_ID_GEN_BASIC:
		return new ZclGenBasic;
//	case ZCL_CLUSTER_ID_GEN_POWER_CFG:
//		return new ZclGenPowerCfg;
//	case ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG:
//		return new ZclGenDevTempCfg;
	case ZCL_CLUSTER_ID_GEN_ON_OFF:
		return new ZclGenOnOff;	
	} // switch
	return NULL;
}


int ZclBase::readAttr( ZigbeeDevice &dev, uint16 clusterId, ZclAttr &attr, int timeout )
{
	return readAttrGroup( dev, clusterId, &attr, 1, timeout );
}


int ZclBase::readAttrGroup( ZigbeeDevice &dev, uint16 clusterId, 
					ZclAttr *attrArray, uint16 arrayLen, int timeout )
{
#if 0
	int 			i, ret = 0;
	uint16			addrData;
	ZigbeeAddr		addr;
	void			*resultPtr = 0;
	std::auto_ptr<ZclFrame>		pResultFrame;
	const uint8 	*pRspData;
	ZclFrame		reqFrame;
	uint16			attrID;
    ZclAttr         *pAttr;
    int             attrDataLen;
    uint8           status;
    ZclAttr         fakeAttr;  // for find_if fail, pAttr must point to an obj
	
	DBG( "ZclBase::readAttrGroup() clusterID = %04x", clusterId );
	
	addrData = dev.addr();
	make_address( &addr, SHORT, &addrData );
	
	reqFrame.setSeqNO( dev.seqNO() );
	reqFrame.setCmd( (uint8)CMD_READ_ATTR );

	for( i = 0; i < arrayLen; ++i ) {
		attrID = (attrArray + i )->id;
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &attrID, uint16 );
#endif
		reqFrame.appendPayload( &attrID, 2 );
	}
	
	ret = zigbee_zcl_cmd( &addr, thisDev->endpoint(), dev.endpoint(), dev.profileId(), clusterId, &reqFrame,
						 timeout, &resultPtr );
	
	if( ret ) {
		DBG( "zigbee_zcl_cmd fail, %d", ret );
		return ret;
	}

	if( !timeout )
		return 0;
	
	if( !resultPtr ) {
		DBG( "callback returned null data." );
		return 1;
	}
	
//	DBG( "ZclBase::readAttrGroup() got callback data." );
	pResultFrame.reset( (ZclFrame*)resultPtr );
//	DBG2( "%s", pResultFrame->toString() );
    
    pRspData = pResultFrame->payload();
    while( pRspData < pResultFrame->payload() + pResultFrame->payloadLen() ) {
        attrID = to_u16( pRspData );
        pRspData += 2;
#ifdef HOST_ZCL_ENDIAN_DIFF
        BYTESWAP( &attrID, uint16 );
#endif
        pAttr = std::find_if( attrArray, attrArray + arrayLen,
                        ZclElemFinder<ZclAttr>(ZclElemFinder<ZclAttr>::BYID, attrID) );
        if( attrArray + arrayLen == pAttr )
            pAttr = &fakeAttr;
        pAttr->status = *pRspData++;
        if( 0 == pAttr->status ) {
            pAttr->dataType = *pRspData++;
            attrDataLen = zclDataTypeLen( pAttr->dataType );
            if( ZCL_DATA_VAR_LEN == attrDataLen ) {
                attrDataLen = *pRspData;        // the 1st byte of content represents length
                pAttr->setData( pRspData, attrDataLen + 1 );
                pRspData += attrDataLen + 1;
            } else {
                pAttr->setData( pRspData, attrDataLen );
                pRspData += attrDataLen;
            }
        } // if status
    } // while
			
	return ret;
#endif // if 0
	return 0;
}


int ZclBase::writeAttrGroup( ZigbeeDevice &dev, uint16 clusterId, 
				ZclAttr *attrArray, uint16 arrayLen, int timeout )
{
#if 0
	int 			i, ret = 0;
	uint16			addrData;
	Address_t		addr;
	void			*resultPtr = 0;
	std::auto_ptr<ZclFrame>		pResultFrame;
	const uint8 	*pRspData;
	ZclFrame		reqFrame;
	uint16			attrID;
	uint8			status;


	DBG( "ZclBase::writeAttrGroup() clusterID = %04x", clusterId );

	addrData = dev.addr();
	make_address( &addr, SHORT, &addrData );

	reqFrame.setSeqNO( dev.seqNO() );
	reqFrame.setCmd( (uint8)CMD_WRITE_ATTR );

	for( i = 0; i < arrayLen; ++i ) {
		attrID = (attrArray + i )->id;
#ifdef HOST_ZCL_ENDIAN_DIFF
		BYTESWAP( &attrID, uint16 );
#endif
		reqFrame.appendPayload( &attrID, 2 );
		reqFrame.appendPayload( (attrArray + i )->dataType );
		reqFrame.appendPayload( (attrArray + i )->data(), (attrArray + i )->dataLen() );
	}

	ret = zigbee_zcl_cmd( &addr, thisDev->endpoint(), dev.endpoint(), dev.profileId(), clusterId, &reqFrame,
						 timeout, &resultPtr );
	
	if( ret ) {
		DBG( "zigbee_zcl_cmd fail, %d", ret );
		return ret;
	}	

	if( !timeout )
		return 0;
	
	if( !resultPtr ) {
		DBG( "callback returned null data." );
		return -1;
	}

	pResultFrame.reset( (ZclFrame*)resultPtr );

	if( !(pResultFrame->payload()) || pResultFrame->payloadLen() % 3 )
		return -1;

	ret = pResultFrame->payloadLen() / 3;
	if( ret != arrayLen )
		return arrayLen - ret;

    pRspData = pResultFrame->payload();
	ret = 0;			// count status fail
    while( pRspData < pResultFrame->payload() + pResultFrame->payloadLen() ) {
		status = *pRspData;
		if( status )
			++ret;
		pRspData += 3;
	} // while

	return ret;
#endif // if 0
	return 0;
}


int ZclBase::writeAttr( ZigbeeDevice &dev, uint16 clusterId, ZclAttr &attr, int timeout )
{
	return writeAttrGroup( dev, clusterId, &attr, 1, timeout );
}



int ZclBase::issueZclCmd( ZigbeeDevice &dev, uint16 clusterID, uint8 cmdID, 
				void *arg, uint16 arglen, int timeout )
{
#if 0
    int 			ret = 0;
    uint16			addrData;
    Address_t		addr;
    void			*resultPtr = 0;
    std::auto_ptr<ZclFrame>		pResultFrame;
    ZclFrame		reqFrame;
    const uint8 	*rspData;
    uint8			rspCmd;
    uint8			rspStatus;
    
    
    DBG( "ZclBasic:issueZclCmd() %04x, %02x", clusterID, cmdID );
    
    reqFrame.setCtrl( ZCL_FRAME_CLUSTER_SPEC );
    reqFrame.setSeqNO( dev.seqNO() );
    reqFrame.setCmd( cmdID );
	if( arg )
		reqFrame.setPayload( arg, arglen );
    
    addrData = dev.addr();
    make_address( &addr, SHORT, &addrData );
    
    ret = zigbee_zcl_cmd( &addr, thisDev->endpoint(), dev.endpoint(), dev.profileId(), clusterID,
                         &reqFrame, timeout, &resultPtr );
    if( ret ) {
        DBG( "zigbee_zcl_cmd fail, %d", ret );
        return ret;
    }
    
    if( !timeout )
        return 0;
    
    if( !resultPtr ) {
        DBG( "callback returned null data." );
		return 1;
    }
    
    DBG( "got callback data:" );
    pResultFrame.reset( (ZclFrame*)resultPtr );
//	DBG2( "%s", pResultFrame->toString() );
    if( pResultFrame->payloadLen() < 2 )
		return 2;
    
    rspData = pResultFrame->payload();
    rspCmd = *rspData++;
    rspStatus = *rspData;
    if( rspCmd != cmdID )
		return 3;
    
    ret = rspStatus;
	
    DBG( "ZclBase::issueZclCmd() returned %d", ret );
    return ret;
#endif // if 0
	return 0;
}


int ZclBase::updateAttr( ZigbeeDevice &dev, uint16 attrID, int timeout )
{
#if 0
	ZclAttrPtr			pAttr;
	ZclAttrPtr			attrList = this->getAttrList();
	uint16				attrListLen = this->getAttrListLen();

	if( !attrList )
		return 0;

	pAttr = std::find_if( attrList, attrList + attrListLen, ZclElemFinder<ZclAttr>(ZclElemFinder<ZclAttr>::BYID, attrID) );

	if( attrList + attrListLen == pAttr )
		return -1;

	return readAttr( dev, clusterID(), *pAttr, timeout );
#endif 
	return 0;
}


int ZclBase::updateAllAttrs( ZigbeeDevice &dev, int timeout )
{
	ZclAttrPtr			attrList = this->getAttrList();
	uint16				attrListLen = this->getAttrListLen();

	if( !attrList )
		return 0;

	return readAttrGroup( dev, clusterID(), attrList, attrListLen, timeout );
}


ZclAttrPtr ZclBase::getAttrLocal( uint16 attrID )
{
	ZclAttrPtr			pAttr;
	ZclAttrPtr			attrList = this->getAttrList();
	uint16				attrListLen = this->getAttrListLen();

	if( !attrList )
		return NULL;

	pAttr = std::find_if( attrList, attrList + attrListLen, ZclElemFinder<ZclAttr>(ZclElemFinder<ZclAttr>::BYID, attrID) );

	if( attrList + attrListLen == pAttr )
		return NULL;

	return pAttr;
	return NULL;
}


void ZclBase::setAttrLocal( uint16 attrID, const void *buf, uint16 len )
{
	ZclAttrPtr			pAttr;
	ZclAttrPtr			attrList = this->getAttrList();
	uint16				attrListLen = this->getAttrListLen();

	if( !attrList )
		return;

	pAttr = std::find_if( attrList, attrList + attrListLen, ZclElemFinder<ZclAttr>(ZclElemFinder<ZclAttr>::BYID, attrID) );

	if( attrList + attrListLen == pAttr )
		return;

	pAttr->setData( buf, len );
}








