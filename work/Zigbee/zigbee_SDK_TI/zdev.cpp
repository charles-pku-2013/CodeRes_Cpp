#include "zigbee_common.h"
#include "zdev.h"
#include "zcl.h"



// members of ZigbeeService
ZigbeeService::ZigbeeService( const ZigbeeDevice *_pDev, uint8 ep, uint16 _profileID ) 
			: pDev(const_cast<ZigbeeDevice*>(_pDev)), endpoint(ep), profileID(_profileID),
			devID(0), devVer(0)
{
	addClusterByID( ZCL_CLUSTER_ID_GEN_BASIC );
}



ZigbeeService::~ZigbeeService()
{
//	AutoWRLock lock(this);
	for( ClusterSet::iterator it = clusters.begin(); it != clusters.end(); ++it )
		delete *it;
}

int ZigbeeService::addCluster( ZclPtr pCluster )
{
	typedef std::pair<ClusterSet::iterator, bool> ClusterSetInsertRet;
	AutoWRLock lock(this);
	ClusterSetInsertRet ret = clusters.insert( pCluster );
	return !(ret.second);
}

int ZigbeeService::addClusterByID( uint16 clusterID )
{
	std::auto_ptr<ZclBase> pZcl( ZclBase::getClusterByID( clusterID ) );
	if( pZcl.get() && addCluster( pZcl.get() ) == 0 )
		pZcl.release();
}

// end members of ZigbeeService




// members of ZigbeeDevice
ZigbeeDevice::ZigbeeDevice() : parent(thisDev.get()), channel(0), state(0) 
{
}

ZigbeeDevice::ZigbeeDevice( const ZigbeeAddr &_addr ) : parent(thisDev.get()), channel(0), state(0), addr(_addr)
{
}


ZigbeeDevice::~ZigbeeDevice()
{
	if( srvTable.size() )
		zigbeeSrvSet.remove_by_dev( this );
}


int ZigbeeDevice::addService( ZigbeeService *pService )
{
	AutoWRLock lock(this);

	std::pair<ZigbeeServiceTable::iterator, bool> retVal = 
			srvTable.insert( ZigbeeServiceTable::value_type(pService->getEndpoint(), pService) );
	//!! insert to global set

	return !(retVal.second);
}

// end members of ZigbeeDevice




