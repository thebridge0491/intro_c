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

echo "configuring build/build.ninja ..."
cat << EOF > build/build.ninja
PREFIX = $prefix
sosuffix = $sosuffix

EOF
if [ 1 = $debug ] ; then
	cat << EOF >> build/build.ninja ;
DEFAULT_CPPFLAGS = -DDEBUG -UNDEBUG $CPPFLAGS
DEFAULT_CFLAGS = -g3 -O0 --coverage $CFLAGS
DEFAULT_LDFLAGS = --coverage $LDFLAGS

EOF
else
	cat << EOF >> build/build.ninja ;
DEFAULT_CPPFLAGS = -DNDEBUG -UDEBUG $CPPFLAGS
DEFAULT_CFLAGS = -O3 $CFLAGS
DEFAULT_LDFLAGS = $LDFLAGS

EOF
fi
cat build.ninja.new >> build/build.ninja

echo "Finished configuring, for help: ninja -C build help"
