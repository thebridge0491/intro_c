#!/bin/sh

prefix=/usr/local
debug=0

if [ build = `basename $PWD` ] ; then
	echo ; echo "ERROR: [sh] ./configure.sh [OPTIONS]" ;
	echo ; exit 1 ;
fi
if [ "`uname -s 2>/dev/null || echo not`" = "Darwin" ] ; then
  sosuffix=dylib
else
  sosuffix=so
  LDFLAGS="$LDFLAGS -Wl,--enable-new-dtags"
fi

mkdir -p build
for opt in "$@" ; do
	case $opt in
	--prefix=) ;;
	--prefix=*) prefix=`echo $opt | sed 's|--prefix=||'` ;;
	--enable-debug) debug=1 ;;
	--disable-debug) debug=0 ;;
	--help)
		echo "Usage: [sh] ./configure.sh [OPTIONS]" ;
		echo "options:" ;
		echo "  --prefix=[${prefix}]: installation prefix" ;
		echo "  --enable-debug: include debug symbols during compile" ;
		echo "  --disable-debug: exclude debug symbols during compile" ;
		exit 0 ;;
	esac
done

echo "configuring build/Rakefile ..."
cat << EOF > build/Rakefile
PREFIX = '$prefix'
SOSUFFIX = '$sosuffix'

EOF
if [ 1 = $debug ] ; then
	cat << EOF >> build/Rakefile ;
DEBUG = '$debug'

ENV['CPPFLAGS'] = "$CPPFLAGS -DDEBUG -UNDEBUG "
ENV['CFLAGS'] = "$CFLAGS -g3 -O0 --coverage "
ENV['LDFLAGS'] = "$LDFLAGS --coverage "

EOF
else
	cat << EOF >> build/Rakefile ;
ENV['CPPFLAGS'] = "$CPPFLAGS -DNDEBUG -UDEBUG "
ENV['CFLAGS'] = "$CFLAGS -O3 "
ENV['LDFLAGS'] = "$LDFLAGS "

EOF
fi
cat Rakefile.new >> build/Rakefile

echo "Finished configuring, for help: make help"
