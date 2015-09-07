#include <windows.h>
#include <stdio.h>
#include <Tlhelp32.h.>
#include <tchar.h>

int main( int argc, char **argv )
{
	FILE *f;

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFile[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR tmp[_MAX_PATH];

	TCHAR *pszFN, *pszSN, *pszTmp;

	BOOL bFN = FALSE;
	BOOL bSN = FALSE;

	HANDLE         hProcessSnap = NULL;
	HANDLE         hModuleSnap  = NULL;
	PROCESSENTRY32 pe32         = { 0 };
	MODULEENTRY32  me32         = { 0 };

	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	me32.dwSize = sizeof( MODULEENTRY32 );
	Process32First( hProcessSnap, &pe32 );

	do
	{
		hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pe32.th32ProcessID );
		Module32First( hModuleSnap, &me32 );

		_tprintf( _T( "%d\n%s\n%s\n%s\n\n" ), pe32.th32ProcessID, pe32.szExeFile, me32.szModule, me32.szExePath );
		
		if( !_tcscmp( _T( "fips.exe" ), ( TCHAR * )_tcslwr( pe32.szExeFile ) ) )
		{
			_tsplitpath( me32.szExePath, szDrive, szDir, szFile, szExt );
			_tprintf( _T( "!! found fips at: %s%s\n\n" ), szDrive, szDir );

			_stprintf( tmp, _T( "%s%sfips.ini" ), szDrive, szDir );
			f = _tfopen( tmp, _T( "r" ) );

			if( f )
			{
				while( _ftscanf( f, _T( "%s" ), tmp ) != EOF )
				{
					pszTmp = ( TCHAR * )_tcslwr( tmp );
					if( _tcsclen( pszTmp ) > 10 )
					{
						if( !_tcsncmp( _T( "firstname=" ), pszTmp, 10 ) )
						{
							pszFN = ( TCHAR * )( pszTmp + 10 );
							_tprintf( _T( "firstname = %s\n" ), pszFN );
							if( !_tcscmp( _T( "alexey" ), pszFN ) )
							{
								bFN = TRUE;
							}
						}

						if( !_tcsncmp( _T( "secondname=" ), pszTmp, 11 ) )
						{
							pszSN = ( TCHAR * )( pszTmp + 11 );
							_tprintf(  _T( "secondname = %s\n" ), pszSN );	
							if( !_tcscmp( _T( "kochubey" ), pszSN ) )
							{
								bSN = TRUE;
							}
						}
					}
				}

				fclose( f );

				if( ( bFN )&&( bSN ) )
				{
					_tprintf( _T( "\n\n!!! FOUND !!!\n\n" ) );
				}
			}
			else
			{
				_tprintf( _T( "error opening %s" ), tmp );
			}

		}
                		

		CloseHandle( hModuleSnap );
	}
	while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );

	return 0;
}
