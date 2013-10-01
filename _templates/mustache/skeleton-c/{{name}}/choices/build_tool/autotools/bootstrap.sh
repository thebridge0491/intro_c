#!/bin/sh

usage="usage: cd build ; ${0} { configure [OPTS] | clean_autoconf }"

if ! [ build = `basename $PWD` ] ; then \
	echo ; echo "ERROR:" ${usage} ; echo ; exit 1 ; \
fi

# Autotools bootstrap script
configure() {
	set -e ; mkdir -p ../m4 #; (cd .. ; autoscan -v -I.)
	#(cd .. ; touch AUTHORS ChangeLog HACKING NEWS README)
		
	# (cd .. ; libtoolize -v -c -i ; aclocal --verbose -Wall -I. -Im4 ; \
	#	autoconf -v -Wall -I. ; autoheader -v -Wall -I. ; \
	#	automake -v -Wall -a -c)
	(cd .. ; libtoolize --verbose -c -f -i ; autoreconf -v -Wall -I. -f -i)
	rm -rf ../autom4te.cache
	
	../configure $@
}

clean_autoconf() {
	cd .. ; rm -rf aclocal.m4 autom4te.cache build-aux configure m4 Makefile.in src/config.h.in
}

if [ "$1" = "configure" ] || [ "$1" = "clean_autoconf" ] ; then
    func=$1 ; shift ; ${func} $@ ;
else
    echo ${usage} ;
fi
