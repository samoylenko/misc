#include <stdio.h>
#include <windows.h>
#include <ras.h>

int main( int argc, char *argv )
{
	LPRASCONN lpRasConn;
	int nRet, i;
	DWORD lpcb, lpcConnections;
	HWND dw;
	HDC hdc;

	lpRasConn = ( LPRASCONN ) GlobalAlloc(GPTR, sizeof(RASCONN));
	lpRasConn->dwSize = sizeof( RASCONN );
 
	nRet = RasEnumConnections( lpRasConn, &lpcb, &lpcConnections );
	if( nRet != 0 )
	{
		printf( "RasEnumConnections failed: Error = %d", nRet );
	}
	else
	{
    		printf( "The following RAS connections are currently active\n\n" );
    		for( i = 0; i < lpcConnections; i++ )
    		{
        		printf( "Entry name: %s\n", lpRasConn->szEntryName );
        		lpRasConn++;
    		}

		if( lpcConnections < 1 )
		{
			printf( "error! no connection!" );

			dw = FindWindow( "#32769", NULL );
			hdc = GetDC( dw );

			TextOut( hdc, 15, 15, "PLEASE PLUG THE NETWORK CABLE BACK IN!", 28 );

			for( i = 0x250; i < 0x2500; i += 0x100 )
			{
				Beep( i, 10 );
			}

			ReleaseDC( dw, hdc );

		}
	
	}
	return 0;
}