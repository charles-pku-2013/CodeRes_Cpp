#include <string>

#include <vector>
#include <stdlib>

typedef enum eNetResult{ 
ZNet_success=0,
	ZNet_password_fail=1,
	ZNet_encrypt_fail=2,
	ZNet_ssid_fail=3,
	ZNet_unknown_fail=4,
}ZNetResult;

typedef enum eWorkMode{ 
AP,
	CLIENT
}ZWorkMode;


typedef enum eNetworkType{ 
ETHERNET, 
	WIFI 
}ZNetType;


typedef enum NetworkMode{ 
	DHCP, 
	STATIC, 
	//PPPOE 
}ZNetMode;

typedef enum eEncryption { 
	OPEN, 
	WEP, 
	WPATKIP,
	WPAAES, 
	WPA2TKIP, 
	WPA2AES
}ZEncryptType;


struct ZAccessPoint 
{
	
	std::string ssid; 
	
	ZEncryptType encryption;
	
	int quality;
	
	int channel;

};


typedef struct s_NetSetting
{
	NetworkMode netMode;
	std::string ipAddr;
	std::string netMask;
	std::string gateWay;
	std::string preferedDNS;
	std::string alternateDNS;
	//wifi
	std::string wifiSSID;
	std::string wifiPassword;
	int wifiEncrypt;
	int wifiChannel;
	//pppoe
	//bool isPPPoE;
	//std::string pppUser;//pppoe
	//std::string pppPassword;//pppoe
	//int pppMode;
	//int pppIdletime;
	std::string macAddr;
}ZNetSetting;


class IZHardware
{
public:	
	
	static IZHardware* GetInstance();
	static void FreeInstance();
	// network
	virtual ZWorkMode GetCurrWorkMode()=0;	// AP OR CLIENT		
	virtual ZNetType GetCurrNetType()=0;		// ETHERNET OR WIFI
	virtual void GetAvailableAPs(vector<ZAccessPoint> & aplist) = 0 ; // all acesspoint information .
	virtual ZNetResult SetNetSetting(ZNetType type,const ZNetSetting setting)=0;
	virtual bool GetNetSetting(ZNetType type,ZNetSetting & setting)=0;
	//while bCleanConfig is true delete all *.xml config files; while bReload is true , kill currents process and reload it.
	virtual bool Switch2ApMode(bool bCleanConfig,bool bReload)=0;
	//get mac we used for Validation
	virtual std::string GetCheckMac()=0;
	//updating
	virtual bool download_image( const std::string &imageURL )=0;
	virtual int write_image()=0;
	virtual void cancel_download_image()=0;
	virtual int get_download_status( int &progress )=0;// retval: -1 downloading, 0 success, 1 error 
	// information
	virtual std::string get_firmware_version()=0;;
	virtual std::string get_manufacturer()=0;;
	virtual std::string get_model()=0;
	//timezone
	bool set_time_zone( int offset_minutes /*-12*60~12*60*/)=0;
	int get_time_zone() const=0;//be frequently called, don't do syscall or flashaccess inside 
    
};