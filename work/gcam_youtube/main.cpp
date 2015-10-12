#include "gcam_youtube.h"
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

static
GcamYoutube obj;
static char *user, *passwd;


void *upload_fun( void *arg )
{
	MediaInfo mediaInfo2( "./icloud.mp4" );
	obj.upload( mediaInfo2 );
	return (void*)0;
}

void *login_fun( void *arg )
{
	bool ret;
	ret = obj.login( user, passwd );
	if( !ret ) {
		printf( "login fail\n" );
	}	
	return (void*)0;
}


int main( int argc, char **argv )
{
	obj.uploadLog( "camera@gaaiho.com", "xxx", 123456, 0, "content" );
	
/*
	user = argv[1];
	passwd = argv[2];
	
	obj.setTimeout( 10 );
*/
/*
	bool ret;
	ret = obj.login( user, passwd );
	if( !ret ) {
		printf( "login fail\n" );
		exit(1);
	}
*/

/*
	pthread_t tid;
	pthread_create( &tid, NULL, login_fun, NULL );
	sleep( 1 );
	obj.cancel();
//	pthread_cancel( tid );
	pause();
*/

/*
	MediaInfo mediaInfo1( "./upload1.mp4" );
	obj.upload( mediaInfo1 );
	
	MediaInfo mediaInfo2( "./upload2.mp4" );
	obj.upload( mediaInfo2 );

	MediaInfo mediaInfo3( "./upload3.mp4" );
	obj.upload( mediaInfo3 );		

	MediaInfo mediaInfo4( "./2012-03-01_16-17-56.avi" );
	obj.upload( mediaInfo4 );	
*/	
	return 0;
}






