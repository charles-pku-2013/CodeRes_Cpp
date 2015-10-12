#include "zigbee.h"


static char						line[256];
static ZigbeeDevice				*warningDev = 0;

static
void handleWarningDemo( ZigbeeDevice *pDev )
{
	time_t now = time(NULL);

	printf( "warningTest...\n" );

	pDev->update();

	switch( pDev->devID() ) {
	case ZCL_HA_DEVICEID_UEBC_AIR_MONITOR:
		{
			UbecAirMonitor *pAirMonitor = dynamic_cast<UbecAirMonitor*>(pDev);
			int warningCode = pAirMonitor->getWarningCode();
			switch( warningCode ) {
			case UbecAirMonitor::WARN_LOW_TEMP:
				printf( "%s UbecAirMonitor low temperature warning! Temperature is %0.2f\n", ctime(&now), pAirMonitor->getTemperature() );
				break;
			case UbecAirMonitor::WARN_HIGH_TEMP:
				printf( "%s UbecAirMonitor high temperature warning! Temperature is %0.2f\n", ctime(&now), pAirMonitor->getTemperature() );
				break;
			case UbecAirMonitor::WARN_HIGH_CO2:
				printf( "%s UbecAirMonitor high CO2 warning! CO2 concentration is %u\n", ctime(&now), pAirMonitor->getCO2Concentration() );
				break;	
			default:
				printf( "%s Unknown warning msg %08x\n", __TIME__, (uint32)warningCode );
			} // switch
		}
		break;
	} // switch

	return;
}

int zigbeeEventHandler( ZigbeeEventType type, ZigbeeDevice *pDev )
{
    DBG( "zigbeeEventHandler invoked..." );
    switch( type ) {
        case ZIGBEE_NEW_JOIN:
            DBG( "New device joined the network.\n%s", pDev->toString() );
            break;
        case ZIGBEE_LEAVE:
            DBG( "Someone left the network.\n%s", pDev->toString() );
            break;
        case ZIGBEE_WARNING:
//            DBG( "WARNING message received.\n%s", pDev->toString() );
			warningDev = pDev;
            break;
    }
    return 0;
}

void test_ubec_plug( UbecPlug *p )
{
	int choice = 0;

	printf( "test_ubec_plug...\n" );

	while( 1 ){
		printf( "please choose operation: \n" );
		printf( "%d:\t\t%s\n", 1, "update");
		printf( "%d:\t\t%s\n", 2, "on" );
		printf( "%d:\t\t%s\n", 3, "off" );
		printf( "%d:\t\t%s\n", 4, "toggle" );
		printf( "%c:\t\t%s\n", 't', "testDemo" );
		printf( "%c:\t\t%s\n", 'q', "quit" );
		
		fgets( line, 256, stdin );
		
		if( *line == 'q' ) {
			printf( "test_ubec_plug end.\n" );
			return;
		} else if( *line == 't' ) {
			p->test();
			return;
		}
		
		choice = atoi(line);
		if( (choice < 1) || (choice > 5) ){
			printf( "WRONG Command!\n" );
			continue;
		} // if
		
		switch(choice){
		case 1:
			p->update( );
			printf( "%s\n", p->toString() );
			break;
		case 2:
			p->on( );
			break;
		case 3:
			p->off( );
			break;
		case 4:
			p->toggle( );
			break;
		default:
			printf( "WRONG Command!\n" );
		} // switch
	}// while

	return;
}


void test_ubec_air_monitor( UbecAirMonitor *p )
{
	int choice = 0;

	printf( "test_ubec_air_monitor...\n" );
	
	while( 1 ){
		printf( "please choose operation: \n" );
		printf( "%d:\t\t%s\n", 1, "update");
		printf( "%c:\t\t%s\n", 't', "testDemo" );
		printf( "%c:\t\t%s\n", 'q', "quit" );
	
		fgets( line, 256, stdin );
		
		if( *line == 'q' ) {
			printf( "test_ubec_air_monitor end.\n" );
			return;
		} else if( *line == 't' ) {
			p->test();
			continue;
		}
		
		choice = atoi(line);
		if( (choice < 1) || (choice > 2) ) {
			printf( "WRONG Command!\n" );
			continue;
		} // if
		
		switch(choice){
		case 1:
			p->update( );
			printf( "%s\n", p->toString() );
			break;
		default:
			printf( "WRONG Command!\n" );
		} // switch
	} // while

	return;
}
void test_ubec_simple_plug( UbecSimplePlug *p )
{
	int choice = 0;

	printf( "test_ubec_simple_plug...\n" );

	while( 1 ){
		printf( "please choose operation: \n" );
		printf( "%d:\t\t%s\n", 1, "update");
		printf( "%d:\t\t%s\n", 2, "on" );
		printf( "%d:\t\t%s\n", 3, "off" );
		printf( "%d:\t\t%s\n", 4, "toggle" );
		printf( "%c:\t\t%s\n", 'q', "quit" );
		
		fgets( line, 256, stdin );
		
		if( *line == 'q' ) {
			printf( "test_ubec_simple_plug end.\n" );
			return;
		}
		
		choice = atoi(line);
		if( (choice < 1) || (choice > 5) ){
			printf( "WRONG Command!\n" );
			continue;
		} // if
		
		switch(choice){
		case 1:
			p->update( );
			printf( "%s\n", p->toString() );
			break;
		case 2:
			p->on( );
			break;
		case 3:
			p->off( );
			break;
		case 4:
			p->toggle( );
			break;
		default:
			printf( "WRONG Command!\n" );
		} // switch
	}// while

	return;
}

void devTest( ZigbeeDevice *ptr )
{
	uint16 devID = ptr->devID();
	
	switch( devID ){
	case ZCL_HA_DEVICEID_UBEC_PLUG:
		{
			UbecPlug *pPlug = dynamic_cast<UbecPlug*>(ptr);
			test_ubec_plug(pPlug);
		}
		break;
	case ZCL_HA_DEVICEID_UEBC_AIR_MONITOR:
		{
			UbecAirMonitor *pAirMonitor= dynamic_cast<UbecAirMonitor*>(ptr);
			test_ubec_air_monitor(pAirMonitor);
		}
		break;
	case ZCL_HA_DEVICEID_UEBC_SIMPLE_PLUG:
		{
			UbecSimplePlug *pSimplePlug= dynamic_cast<UbecSimplePlug*>(ptr);
			test_ubec_simple_plug(pSimplePlug);
		}
		break;
	default:
		DBG( "ZCL_HA_DEVICEID_UNKNOWN" );
		break;
	}

	return;
}


void printDevList( const std::vector<ZigbeeDevicePtr> &devList )
{
	using namespace std;
	int i = 0;
	
	for( vector<ZigbeeDevicePtr>::const_iterator it = devList.begin(); it != devList.end(); ++it )
		printf( "DEVICE\t\t\t%d\n%s\n", ++i, (*it)->toString() );
}

static
void waitForWarning()
{
	printf( "waitForWarning.\n" );

	while( 1 ) {
		if( warningDev ) {
			handleWarningDemo( warningDev );
			warningDev = 0;
		}
		sleep(1);	
	} // while
}


//static
//int find_op_test( int *pInt, void *arg )
//{
//	printf( "data is %d, arg is %s\n", *pInt, arg ? "not NULL" : "NULL" );
//	return 0;
//}

static
void testBasic()
{
	using namespace std;

//#ifdef _DEBUG
//	zigbeelog1 = fopen( "/tmp/zigbeeLog1", "w" );
//	setbuf( zigbeelog1, NULL ); 
//	zigbeelog2 = fopen( "/tmp/zigbeeLog2", "w" );
//	setbuf( zigbeelog2, NULL ); 
//#endif	

	printf( "%u\n", sizeof(GeneralCallback) );
}


int main( int argc, char **argv )
{
	using namespace std;

	int 						ret, index;
	int 						i = 0;
	uint16 						devID = 0;
	vector<ZigbeeDevicePtr> 	devList;
	ZigbeeDevice				*pDev;

//	testBasic();
//	return 0;

	zigbee_register_event_callback( zigbeeEventHandler );
	
	ret = zigbee_init( argv[1], 115200, 0, 8, 1, 'N' );
	if( ret != 0){
		printf( "zigbee init error %s\n", strerror(errno) );
		exit(-1);
	}

	setbuf( stdout, NULL );
	
//	testUtil();
//	WAIT( "testUtil end." );
//	goto END;

	printf( "Zigbee test starts, please input your choice:\n" );

	while( 1 ) {
		zigbee_get_dev_list( devList ); 		
		printf( "found %d devices in current network:\n", devList.size());	
		printDevList( devList );
		printf( "u = update device list\n" );
		printf( "q = quit the test\n" );
		printf( "w = wait for warning message\n" );
		printf( "number = select a device to test\n" );
		fgets( line, 256, stdin );
		
		if( isdigit(*line) ) {		
			index = atoi(line);
			if( index <= 0 || index > devList.size() ) {
				printf( "WRONG Command!\n" );
				continue;
			} else {
				pDev = devList[index-1];
				devTest(pDev);
			} // if index
		} else {
			switch(*line) {
				case 'u':			
					zigbee_update_dev_list();
					printf( "Press Enter to end device finding..." );
					getchar();
					continue;
				case 'w':
					waitForWarning();
					continue;
				case 'q':
					goto END;
				default:
					printf( "WRONG Command!\n" );
					continue;
			} // switch
		} // if line
	}// while


END:
	printf( "zigbee test end!\n" );
	zigbee_finalize();
	return 0;
}







