#!/bin/sh

prefix=/usr/local
debug=0

if ! [ build = `basename $PWD` ] ; then
	echo ; echo "ERROR: cd build ; [sh] ../configure.sh [OPTIONS]" ;
	echo ; exit 1 ;
fi

for opt in "$@" ; do
	case $opt in
	--prefix=) ;;
	--prefix=*) prefix=`echo $opt | sed 's|--prefix=||'` ;;
	--enable-debug) debug=1 ;;
	--disable-debug) debug=0 ;;
	--help)
		echo "Usage: cd build ; [sh] ../configure.sh [OPTIONS]" ;
		echo "options:" ;
		echo "  --prefix=[${prefix}]: installation prefix" ;
		echo "  --enable-debug: include debug symbols during compile" ;
		echo "  --disable-debug: exclude debug symbols during compile" ;
		exit 0 ;;
	esac
done

echo "configuring rakefile ..."
cat << EOF > rakefile
PREFIX = ENV['prefix'] ? ENV['prefix'] : '$prefix'
DEBUG = ENV['DEBUG'] ? ENV['DEBUG'] : '$debug'

EOF
cat ../rakefile.new >> rakefile

echo "Finished configuring, for help: make help"
