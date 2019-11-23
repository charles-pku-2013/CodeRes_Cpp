#include "gcam_youtube.h"
#include <string>
#include <cstring>
#include <sstream>
#include <tinyxml.h>

using namespace std;

const char *MediaInfo::category = "Film";

MediaInfo::MediaInfo( const string &path ) : filepath(path)
{
    // set title
    string::size_type p, q;
    string::iterator first, last;
    p = filepath.rfind('.');
    if( string::npos == p )
        last = filepath.end();
    else
        last = filepath.begin() + p;
    q = filepath.rfind('/', p);
    if( string::npos == q )
        first = filepath.begin();
    else
        first = filepath.begin() + q + 1;
/*      
    title = "Gcam-AutoRecord-";
    title.append( first, last );
*/
    title = string( first, last );  

    generate_xml();
}

void MediaInfo::generate_xml()
{
    TiXmlDocument doc;
    TiXmlText *text;

    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );

    TiXmlElement *entry = new TiXmlElement( "entry" );
    entry->SetAttribute( "xmlns", "http://www.w3.org/2005/Atom" );
    entry->SetAttribute( "xmlns:media", "http://search.yahoo.com/mrss/" );
    entry->SetAttribute( "xmlns:yt", "http://gdata.youtube.com/schemas/2007" );
    doc.LinkEndChild( entry );

    TiXmlElement *mediaGroup = new TiXmlElement( "media:group" );
    entry->LinkEndChild( mediaGroup );

    TiXmlElement *mediaTitle = new TiXmlElement( "media:title" );
    mediaTitle->SetAttribute( "type", "plain" );
    text = new TiXmlText( title );
    mediaTitle->LinkEndChild( text );
    mediaGroup->LinkEndChild( mediaTitle );

    TiXmlElement *mediaCategory = new TiXmlElement( "media:category" );
    mediaCategory->SetAttribute( "scheme", "http://gdata.youtube.com/schemas/2007/categories.cat" );
    text = new TiXmlText( category );
    mediaCategory-> LinkEndChild( text );
    mediaGroup->LinkEndChild( mediaCategory );
    
    TiXmlElement *privateMark = new TiXmlElement( "yt:private" );
    mediaGroup->LinkEndChild( privateMark );

    stringstream out;
    out << doc;
//  xmlstring.swap( out.str() );
    xmlstring = out.str();

    DEBUG( "%s", xmlstring.c_str() );
}

long GcamYoutube::timeout = 0;

size_t GcamYoutube::callback_func(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t len = size*nmemb;
    string *s = (string*)userp;
    s->append( (char*)buffer, len );
    return len;
}

bool GcamYoutube::login( const string &_username, const string &_password )
{
    DEBUG( "GcamYoutube::login" );

    username = _username;
    password = _password;
    
    CURLcode res;
    smart_ptr<struct curl_slist, CurlSlistFree> chunk( curl_slist_free_all );
    smart_ptr<CURL, CurlFree> curl( curl_easy_cleanup );
    
    curl = curl_easy_init();
    if( !curl.get() ) {
        DEBUG( "curl init fail" );
        return false;
    }
    
    curlPtr = &curl;

    // set url
    curl_easy_setopt( curl.get(), CURLOPT_URL, LOGIN_URL );
    // set ssl
    curl_easy_setopt( curl.get(), CURLOPT_SSL_VERIFYPEER, 0L );
    curl_easy_setopt( curl.get(), CURLOPT_SSL_VERIFYHOST, 0L );
    // set timeout
    if( !timeout )
        curl_easy_setopt( curl.get(), CURLOPT_TIMEOUT, timeout );

    // set header
    chunk = curl_slist_append( chunk.get(), "Content-Type: application/x-www-form-urlencoded" );
    curl_easy_setopt( curl.get(), CURLOPT_HTTPHEADER, chunk.get() );
    // set post fields
    string request( username.length()+password.length()+80, 0 );
    sprintf( &request[0], "Email=%s&Passwd=%s&service=youtube&source=youtube", username.c_str(), password.c_str() );
    curl_easy_setopt( curl.get(), CURLOPT_POST, 1L );
    curl_easy_setopt( curl.get(), CURLOPT_POSTFIELDS, request.c_str() );

    // set callback
    string response;
    curl_easy_setopt( curl.get(), CURLOPT_WRITEFUNCTION, callback_func );
    curl_easy_setopt( curl.get(), CURLOPT_WRITEDATA, &response );

    // apply
    res = curl_easy_perform(curl.get());

    if( res != CURLE_OK ) {
        DEBUG( "%s", curl_easy_strerror(res) );
        return false;
    }

    string::size_type p = response.rfind( "Auth=" );
    if( string::npos == p ) {
        DEBUG( "login request fail" );
        return false;
    }

    auth = strtok( &response[p+5], "\r\n" );    // remove trailing spaces!!!
//  DEBUG( "auth=%s", auth.c_str() );
    return true;
}

bool GcamYoutube::upload( const MediaInfo &mediaInfo )
{
    if( auth.empty() ) {
        DEBUG( "you must first log in" );
        return false;
    }

    CURLcode res;
    smart_ptr<struct curl_slist, CurlSlistFree> chunk( curl_slist_free_all );
    smart_ptr<struct curl_httppost, CurlFormFree> formpost( curl_formfree );
    struct curl_httppost *lastptr=NULL; 
    smart_ptr<CURL, CurlFree> curl( curl_easy_cleanup );
    
    curl = curl_easy_init();
    if( !(curl.get()) ) {
        DEBUG( "curl init fail" );
        return false;
    }
    
    curlPtr = &curl;    

    curl_easy_setopt(curl.get(), CURLOPT_URL, UPLOAD_URL );
    // set timeout
    if( !timeout )
        curl_easy_setopt( curl.get(), CURLOPT_TIMEOUT, timeout );

//  DEBUG( "%s", mediaInfo.xmlstring.c_str() );

    curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "xmlcode",
                CURLFORM_COPYCONTENTS, mediaInfo.xmlstring.c_str(),
                CURLFORM_CONTENTTYPE, "application/atom+xml; charset=UTF-8",
                CURLFORM_END);

    curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, mediaInfo.filepath.c_str(),
                CURLFORM_CONTENTTYPE,"video/avi",
                CURLFORM_END);

    curl_easy_setopt(curl.get(), CURLOPT_HTTPPOST, formpost.get());

    // set header
    char buf[1024];
    sprintf( buf, "Authorization: GoogleLogin auth=%s", auth.c_str() );
    chunk = curl_slist_append(chunk.get(), buf );
    chunk = curl_slist_append(chunk.get(), "GData-Version: 2" );
    sprintf( buf, "X-GData-Key: key=%s", DEV_KEY );
    chunk = curl_slist_append(chunk.get(), buf );
    sprintf( buf, "Slug: %s", mediaInfo.filepath.c_str() );
    chunk = curl_slist_append(chunk.get(), buf );
    chunk = curl_slist_append(chunk.get(), "Content-Type: multipart/related" );
    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, chunk.get());

    string response;
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, callback_func);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);
    res = curl_easy_perform(curl.get());    

    if( res != CURLE_OK ) {
        DEBUG( "%s", curl_easy_strerror(res) );
        return false;
    }

//  DEBUG( "upload_response: %s", response.c_str() );
    if( !parse_upload_response(response) ) {
        DEBUG( "upload fail, server refused" );
        return false;
    }

    return true;
}

bool GcamYoutube::parse_upload_response( const string &xmlstring )
{
    TiXmlDocument doc;
    doc.Parse( xmlstring.c_str() );
    if( doc.Error() ) {
        DEBUG( "unrecogonized response information" );
        return false;
    }
    TiXmlElement *root = doc.RootElement();
    if( !root ) {
        DEBUG( "unrecogonized response information" );
        return false;
    }
//  DEBUG( "ValueStr: %s", root->ValueStr().c_str() );
    if( root->ValueStr() == "errors" )
        return false;
    return true;
}

void GcamYoutube::cancel()
{
    DEBUG( "GcamYoutube::cancel" );
    if( curlPtr )
        curl_easy_cleanup( *curlPtr );
//  *curlPtr = 0;
    curlPtr = 0;
    return;
}


bool GcamYoutube::uploadLog( const std::string &account, const std::string &res, unsigned int appid, 
    int error, const char *log )
{
    DEBUG( "uploading log" );

    CURLcode resno;
    smart_ptr<CURL, CurlFree> curl( curl_easy_cleanup );
    
    curl = curl_easy_init();
    if( !curl.get() ) {
        DEBUG( "curl init fail" );
        return false;
    }
    
    curl_easy_setopt( curl.get(), CURLOPT_URL, LOG_UPLOAD_URL );
    curl_easy_setopt( curl.get(), CURLOPT_USERAGENT, LOG_UPLOAD_AGENT );
    
    string sstr( strlen(log)+256, 0 );
    sprintf( &sstr[0], "account=%s&appid=%u&res=%s&error=%d&ext=log&%s", account.c_str(), 
        appid, res.c_str(), error, log );
    
//  DEBUG( "%s", sstr.str().c_str() );
    
    curl_easy_setopt( curl.get(), CURLOPT_POST, 1L );
    curl_easy_setopt( curl.get(), CURLOPT_POSTFIELDS, sstr.c_str() );   
    
    // set callback
    string response;
    curl_easy_setopt( curl.get(), CURLOPT_WRITEFUNCTION, callback_func );
    curl_easy_setopt( curl.get(), CURLOPT_WRITEDATA, &response );
    
    // apply
    resno = curl_easy_perform(curl.get());

    if( resno != CURLE_OK ) {
        DEBUG( "%s", curl_easy_strerror(resno) );
        return false;
    }
    
    DEBUG( "UPLOAD SUCCESS!!! %s", response.c_str() );
    
    return true;
}









