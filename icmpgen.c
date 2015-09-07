#include <winsock2.h>
#include <stdio.h>

#pragma comment( lib, "ws2_32.lib" )

int main( int argc, char *argv[] )
{
        WSADATA wsaData;
        WORD wVersionRequested;

	unsigned char data[] = 
	{
		0x00, 0x50, 0x56, 0xD8, 0xE0, 0x50, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
		0x00, 0x3C, 0x36, 0x1E, 0x00, 0x00, 0x80, 0x01, 0x83, 0x4F, 0xC0, 0xA8, 0x00, 0x01, 0xC0, 0xA8,
		0x00, 0x02, 0x08, 0x00, 0x3D, 0x5C, 0x03, 0x00, 0x0D, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
		0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
		0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69 
	};

	SOCKET s;
	int on = 1;

        struct hostent *hp;
        struct sockaddr_in dst;


        wVersionRequested = MAKEWORD( 1, 1 );
        if( WSAStartup( wVersionRequested, &wsaData ) < 0 )
	{
		printf( "WSAStartup error\n" );
		exit( 1 );
	}

	s = socket( AF_INET, SOCK_RAW, IPPROTO_IP );
	if( s == INVALID_SOCKET )
	{
		printf( "socket error\n" );
		exit( 1 );
	}

        if( setsockopt( s, IPPROTO_IP, 3, &on, sizeof( on ) ) < 0 ) 
	{
                printf( "setsockopt error\n" );
                exit( 1 );
        }

        if( ( hp = gethostbyname( argv[1] ) ) == NULL ) 
	{
        	if( ( ip->ip_dst.s_addr = inet_addr( argv[1] ) ) == -1 )
		{
                        printf( "%s: unknown host\n", argv[1] );
			exit( 1 );
                }
        } 
	else
	{
                bcopy( hp->h_addr_list[0], &ip->ip_dst.s_addr, hp->h_length );
        }


        WSACleanup();

	return 0;
}