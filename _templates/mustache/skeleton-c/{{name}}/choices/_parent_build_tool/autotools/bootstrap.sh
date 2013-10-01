#!/bin/sh

usage="usage: cd build ; ${0} { configure [OPTS] | clean_autoconf }"

if ! [ build = `basename $PWD` ] ; then \
	echo ; echo "ERROR:" ${usage} ; echo ; exit 1 ; \
fi
SUBDIRS=`if [ -z "$SUBDIRS" ] ; then echo common app ; else echo $SUBDIRS ; fi`

# Autotools bootstrap script
configure() {
	set -e
	for dirX in . $SUBDIRS ; do
		mkdir -p ../$dirX/m4 ;
		#touch ../$dirX/AUTHORS ../$dirX/ChangeLog ../$dirX/HACKING ../$dirX/NEWS ../$dirX/README ;
	done
	# (cd .. ; autoscan -v -I.)
	
	# (cd .. ; libtoolize -v -c -i ; aclocal --verbose -Wall -I. -Im4 ; \
	#	autoconf -v -Wall -I. ; autoheader -v -Wall -I. ; \
	#	automake -v -Wall -a -c)
	(cd .. ; libtoolize --verbose -c -f -i ; autoreconf -v -Wall -I. -f -i)
	for dirX in . $SUBDIRS ; do
		rm -rf ../$dirX/autom4te.cache ;
	done
	
	../configure $@
}

clean_autoconf() {
	for dirX in . $SUBDIRS ; do
		(cd ../$dirX ; rm -rf aclocal.m4 autom4te.cache build-aux configure m4 Makefile.in src/config.h.in) ;
	done
}

if [ "$1" = "configure" ] || [ "$1" = "clean_autoconf" ] ; then
    func=$1 ; shift ; ${func} $@ ;
else
    echo ${usage} ;
fi
