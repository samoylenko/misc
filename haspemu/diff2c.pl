#/usr/bin/perl -e

$i = 0;

while( $buff = <> )
{
	if( ( $offset, $orig, $new ) = ( $buff =~ /(\S+):\s+(\S+)\s+(\S+)/ )  )
	{
		print "0x$offset, 0x$orig, 0x$new,\n";
		$i++;
	}
}

print "plen = $i\n"