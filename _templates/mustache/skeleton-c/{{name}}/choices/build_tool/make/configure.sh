#!/bin/sh

prefix=/usr/local
srcdir=..
vpath=..
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
	--srcdir=) ;;
	--prefix=*) prefix=`echo $opt | sed 's|--prefix=||'` ;;
	--srcdir=*) srcdir=`echo $opt | sed 's|--srcdir=||'` ; vpath=${srcdir} ;;
	--enable-debug) debug=1 ;;
	--disable-debug) debug=0 ;;
	--help)
		echo "Usage: [sh] ./configure.sh [OPTIONS]" ;
		echo "options:" ;
		echo "  --prefix=[${prefix}]: installation prefix" ;
		echo "  --srcdir=[${srcdir}]: source code directory" ;
		echo "  --enable-debug: include debug symbols during compile" ;
		echo "  --disable-debug: exclude debug symbols during compile" ;
		exit 0 ;;
	esac
done

echo "configuring build/Makefile ..."
cat << EOF > build/Makefile
.POSIX:
.SUFFIXES:
PREFIX = $prefix
VPATH = $vpath
sosuffix = $sosuffix

EOF
if [ 1 = $debug ] ; then
	cat << EOF >> build/Makefile ;
DEBUG = $debug

CPPFLAGS = $CPPFLAGS -DDEBUG -UNDEBUG
CFLAGS = $CFLAGS -g3 -O0 --coverage
LDFLAGS = $LDFLAGS --coverage

EOF
else
	cat << EOF >> build/Makefile ;
CPPFLAGS = $CPPFLAGS -DNDEBUG -UDEBUG
CFLAGS = $CFLAGS -O3
LDFLAGS = $LDFLAGS

EOF
fi
cat Makefile.new >> build/Makefile

echo "Finished configuring, for help: make -C build help"
