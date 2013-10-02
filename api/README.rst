Intro_c-Practice
===========================================
.. .rst to .html: rst2html5 foo.rst > foo.html
..                pandoc -s -f rst -t html5 -o foo.html foo.rst

Practice sub-package for C Intro examples project.

Installation
------------
source code tarball download:
    
        # [aria2c --check-certificate=false | wget --no-check-certificate | curl -kOL]
        
        FETCHCMD='aria2c --check-certificate=false'
        
        $FETCHCMD https://bitbucket.org/thebridge0491/intro_c/[get | archive]/master.zip

version control repository clone:
        
        git clone https://bitbucket.org/thebridge0491/intro_c.git

build example with gradle:
cd <path> ; gradle assemble [check]

[sudo] gradle [-Pprefix=$PREFIX] install

build example with premake:
cd <path>/build ; premake4 --file=../premake4.lua gmake [--help]

make all [; premake4 --file=../premake4.lua test]

[sudo] premake --file=../premake4.lua [--prefix=$PREFIX] install

build example with rake:
cd <path>/build ; [sh] ../configure.sh [--prefix=$PREFIX] [--help]

rake all [test]

[sudo] rake install

build example with scons:
cd <path> ; scons [; scons test]

[sudo] scons install [prefix=$PREFIX]

build example with make:
cd <path>/build ; [sh] ../configure.sh [--prefix=$PREFIX] [--help]

make all [check]

[sudo] make install

build example with autotools:
cd <path>/build ; [sh] ../bootstrap.sh [--prefix=$PREFIX] [--help]

make all [check]

[sudo] make install

build example with cmake:
cd <path>/build ; cmake [-DCMAKE_INSTALL_PREFIX=$PREFIX] [-G Ninja] [--help] ..

cmake --build . --target all [; cmake --build . --target test]

Usage
-----
        // PKG_CONFIG='pkg-config --with-path=$PREFIX/lib/pkgconfig'
        
        // $PKG_CONFIG --cflags --libs <ffi-lib>

        #include "intro_c/classic.h"
        
        int n = 5;
        
        unsigned long res = fact_i(n);

Author/Copyright
----------------
Copyright (c) 2013 by thebridge0491 <thebridge0491-codelab@yahoo.com>

License
-------
Licensed under the Apache-2.0 License. See LICENSE for details.
