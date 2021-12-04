Intro_c-Intro
===========================================
.. .rst to .html: rst2html5 foo.rst > foo.html
..                pandoc -s -f rst -t html5 -o foo.html foo.rst

Main app sub-package for C Intro examples project.

Installation
------------
source code tarball download:
    
        # [aria2c --check-certificate=false | wget --no-check-certificate | curl -kOL]
        
        FETCHCMD='aria2c --check-certificate=false'
        
        $FETCHCMD https://bitbucket.org/thebridge0491/intro_c/[get | archive]/master.zip

version control repository clone:
        
        git clone https://bitbucket.org/thebridge0491/intro_c.git

build example with gradle(native plugins):

        // no configure step

        gradle assemble [check]

        [sudo] gradle [-Pprefix=$PREFIX] install

build example with scons:

        // no configure step

        scons [; scons test]

        [sudo] scons install [prefix=$PREFIX]

build example with rake:

        [sh] ./configure.sh [--prefix=$PREFIX] [--help]

        rake -C <dir> all [test]

        [sudo] rake -C <dir> install

build example with ninja:

        [sh] ./configure.sh [--prefix=$PREFIX] [--help]

        ninja -C <dir> [test]

        [sudo] ninja -C <dir> install

build example with make:

        [sh] ./configure.sh [--prefix=$PREFIX] [--help]

        make -C <dir> all [test]

        [sudo] make -C <dir> install

build example with premake:

        cd <dir> ; premake --file=../premake.lua [--prefix=$PREFIX] gmake [--help]

        premake --file=../premake.lua all [; premake --file=../premake.lua test]

        [sudo] premake --file=../premake.lua [--prefix=$PREFIX] install

build example with autotools:

        cd <dir> ; [sh] ../bootstrap.sh configure [--prefix=$PREFIX] [--help]

        make all [check]

        [sudo] make install

build example with meson:

        meson setup [--prefix=$PREFIX] [-h] <dir>

        meson compile -C <dir> [; meson test -C <dir>]

        [sudo] meson install -C <dir>

build example with cmake:

        cmake -B <dir> [-DCMAKE_INSTALL_PREFIX=$PREFIX] [-G Ninja] [--help]

        cmake --build <dir> --target all [; cmake --build <dir> --target test]

        [sudo] cmake --build <dir> --target install

Usage
-----
        [env RSRC_PATH=<path>/resources] [$PREFIX/bin/]intro_c-intro [-h]

Author/Copyright
----------------
Copyright (c) 2013 by thebridge0491 <thebridge0491-codelab@yahoo.com>

License
-------
Licensed under the Apache-2.0 License. See LICENSE for details.
