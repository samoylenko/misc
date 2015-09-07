#include <stdio.h>
#include <winsock2.h>

#pragma comment( lib, "ws2_32.lib" )

#define PORT 6677
#define BUFSIZE 0x200

void showdata( char *buf )
{
	printf( "received: %s\n", buf );
        ZeroMemory( buf, BUFSIZE );
}

int main( int argc, char **argv )
{
    WSADATA wsaData;
    WORD wVersionRequested;
    SOCKET s;
    struct sockaddr_in sa;
    char buf[BUFSIZE];
    int len;

    wVersionRequested = MAKEWORD(1, 1);
    if( WSAStartup( wVersionRequested, &wsaData ) < 0 ) return 1;

    if( ( s = socket( AF_INET, SOCK_DGRAM, 0 ) ) == INVALID_SOCKET )
    {
	perror( "socket" );
	WSACleanup();
        return 1;
    }

    ZeroMemory( &sa, sizeof( struct sockaddr_in ) );

    sa.sin_family       = AF_INET;
    sa.sin_addr.s_addr  = htonl( INADDR_ANY );
    sa.sin_port         = htons( PORT );

    if( bind( s, ( const struct sockaddr * )&sa, sizeof( struct sockaddr_in ) ) )
    {

	perror( "bind" );
        WSACleanup();
        return 1;
    }

    printf( "udp server started on port %d. press ctrl+c to stop //pm\n\n", PORT );

    ZeroMemory( buf, BUFSIZE );
    for( ; ; )
    {
    	if( len = recv( s, buf, BUFSIZE, 0 ) )
	{
		showdata( buf );
        }

    }
}