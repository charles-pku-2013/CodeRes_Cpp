#ifndef GCAM_YOUTUBE_H
#define GCAM_YOUTUBE_H

#include <string>
#include <curl/curl.h>

#ifdef _DEBUG
	#define DEBUG( format, ... ) fprintf( stderr, format"\n", ##__VA_ARGS__ )
#else
	#define DEBUG
#endif

#ifndef LINE_MAX
	#define LINE_MAX sysconf(_SC_LINE_MAX)
#endif

#define LOGIN_URL "https://www.google.com/accounts/ClientLogin"
#define UPLOAD_URL "http://uploads.gdata.youtube.com/feeds/api/users/default/uploads"
#define DEV_KEY "AI39si7HzRPtCoCALfJeNLSldcBobeaTMZzdqUuAIRY4VzcUA2p0kmjdE9KLwDHP7ULTvH5yzDe4X-L_OAVlUNmyUSjCZBl2Jw"

#define LOG_UPLOAD_AGENT "GaaihoCamera v=10E9E1293D9767CD18E118A855C06392"
#define LOG_UPLOAD_URL "http://camera.gaaiho.com/LogCollector.aspx"

class MediaInfo {
	friend class GcamYoutube;
public:
	MediaInfo( const std::string &path );

private:
	std::string filepath;
	std::string title;
	std::string xmlstring;
private:
	void generate_xml();
private:
	static const char *category;
};

template<typename T, typename D>
class smart_ptr {
public:
	smart_ptr( D d ) : ptr(0), release(d) {}
	smart_ptr( T *rhs, D d ) : ptr(rhs), release(d) {}
	smart_ptr &operator = ( T *rhs )
	{
		ptr = rhs;
		return *this;
	}
	~smart_ptr() { release(ptr); }
	T *get() const { return ptr; }
	T** operator & () { return &ptr; }
	void free() { release(ptr); ptr = 0; }
private:
	T *ptr;
	D release;
};

class GcamYoutube {
	typedef void (*CurlFree)( CURL* );
	typedef void (*CurlSlistFree)( struct curl_slist* );
	typedef void (*CurlFormFree)( struct curl_httppost* );
public:
	GcamYoutube() : auth(""), curlPtr(0) {}
	bool login( const std::string &_username, const std::string &_password );
	bool upload( const MediaInfo &mediaInfo );
	void cancel();
	void setTimeout( long val ) { timeout = val; }
public:
	bool uploadLog( const std::string &account, const std::string &res, unsigned int appid, 
		int errno, const char *log );
private:
	std::string username;
	std::string password;
	std::string auth;
	static long timeout;
	CURL **curlPtr;
private:
	static size_t callback_func(void *buffer, size_t size, size_t nmemb, void *userp);
	static bool parse_upload_response( const std::string &xmlstring );
};

#endif	// GCAM_YOUTUBE_H








