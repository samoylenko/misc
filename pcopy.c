#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv )
{
	FILE *f1, *f2;
	char buff[256];

	printf( "file1 \t\t= %s\nfile2\t\t= %s\noffset\t\t= %s\nlength\t\t= %s", argv[1], argv[2], argv[3], argv[4] );

	f1 = fopen( argv[1], "r+b" );
	f2 = fopen( argv[2], "rb" );

	fseek( f2, 0, SEEK_SET );
	fread( buff, 1, atoi( argv[4] ), f2 );
	fseek( f1, atoi( argv[3] ), SEEK_SET );
	fwrite( buff, 1, atoi( argv[4] ), f1 );

	fclose( f1 );
	fclose( f2 );
	

	return 0;
}