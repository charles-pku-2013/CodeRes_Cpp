#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <curl/curl.h>

#define UPLOAD_URL "http://uploads.gdata.youtube.com/feeds/api/users/default/uploads"
#define CLIENT_ID "501386559132.apps.googleusercontent.com"
#define DEV_KEY "AI39si7HzRPtCoCALfJeNLSldcBobeaTMZzdqUuAIRY4VzcUA2p0kmjdE9KLwDHP7ULTvH5yzDe4X-L_OAVlUNmyUSjCZBl2Jw"

using namespace std;

struct AuthInfo {
    AuthInfo() {}
    AuthInfo( const string &s )
    {
        int index = 1;
        for( char *p = strtok((char*)(s.c_str()), " \t\f\r\v\n="); p; p = strtok(NULL, " \t\f\r\v\n=") ) {
            switch( index ) {
            case 2:
                sid = p;
                break;
            case 4:
                lsid = p;
                break;
            case 6:
                auth = p;
                break;
            }
            ++index;
        } // for
    }

    string sid;
    string lsid;
    string auth;
};

CURL *curl;
CURLcode res;
AuthInfo info;

static
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t len = size*nmemb;
    string *s = (string*)userp;
    s->append( (char*)buffer, len );
    return len;
}

void upload_file( const char* filename )
{
    curl = curl_easy_init();
    if( !curl ) return;

    char buf[1024];
    curl_easy_setopt(curl, CURLOPT_URL, UPLOAD_URL );

    FILE *fp;
    if( (fp = fopen("video.xml", "r" )) == NULL ) {
        printf( "cannot open xml file\n" );
        exit(1);
    }
    fseek( fp, 0, SEEK_END );
    size_t filesize = ftell( fp );
    rewind( fp );
    char *xmlcontent = new char[filesize+1];
    memset( xmlcontent, 0, filesize+1 );
    fread( xmlcontent, filesize, 1, fp );
    fclose( fp );
    printf( "%s\n", xmlcontent );

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;

//  curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "name_for_ptrcontent",
//              CURLFORM_PTRCONTENTS, buf, CURLFORM_END);

    curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "xmlcode",
                CURLFORM_COPYCONTENTS, xmlcontent,
                CURLFORM_CONTENTTYPE, "application/atom+xml; charset=UTF-8",
                CURLFORM_END);

    delete [] xmlcontent;

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, filename,
               CURLFORM_CONTENTTYPE,"video/mp4",
               CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    // header
    struct curl_slist *chunk = NULL;
    sprintf( buf, "Authorization: GoogleLogin auth=%s", info.auth.c_str() );
    chunk = curl_slist_append(chunk, buf );
    chunk = curl_slist_append(chunk, "GData-Version: 2" );
    sprintf( buf, "X-GData-Key: key=%s", DEV_KEY );
    chunk = curl_slist_append(chunk, buf );
    sprintf( buf, "Slug: %s", filename );
    chunk = curl_slist_append(chunk, buf );
    chunk = curl_slist_append(chunk, "Content-Type: multipart/related" );
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_formfree(formpost);
    curl_slist_free_all(chunk);
}

int main( int argc, char **argv )
{
    if( argc != 4 ) {
            printf( "usage: ./program username password filename\n" );
            exit(1);
    }

    char *username = argv[1];
    char *password = argv[2];
    char *filename = argv[3];

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;

        // set url
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/accounts/ClientLogin");

        // set ssl
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

//      curl_easy_setopt ( curl, CURLOPT_CAINFO, "ca_bundle.crt");

        // set header
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        // set post fields
        char buf[1024];
        sprintf( buf, "Email=%s&Passwd=%s&service=youtube&source=youtube", username, password );
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);

        // set callback
        string data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        // apply
        res = curl_easy_perform(curl);

        printf( "%s\n", data.c_str() );
        info = AuthInfo( data );
//      printf( "sid=%s\nlsid=%s\nauth=%s\n", info.sid.c_str(), info.lsid.c_str(), info.auth.c_str() );

        curl_easy_cleanup(curl);
    }

    upload_file( filename   );
    return 0;
}
