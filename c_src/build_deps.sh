#!/bin/sh

# /bin/sh on Solaris is not a POSIX compatible shell, but /usr/bin/ksh is.
if [ `uname -s` = 'SunOS' -a "${POSIX_SHELL}" != "true" ]; then
    POSIX_SHELL="true"
    export POSIX_SHELL
    exec /usr/bin/ksh $0 $@
fi
unset POSIX_SHELL # clear it so if we invoke other scripts, they run as ksh as well

MARIO_VSN="1.0.0"


set -e

if [ `basename $PWD` != "c_src" ]; then
    # originally "pushd c_src" of bash
    # but no need to use directory stack push here
    cd c_src
fi

BASEDIR="$PWD"

# detecting gmake and if exists use it
# if not use make
# (code from github.com/tuncer/re2/c_src/build_deps.sh
which gmake 1>/dev/null 2>/dev/null && MAKE=gmake
MAKE=${MAKE:-make}

# Changed "make" to $MAKE

case "$1" in
    rm-deps)
        ;;

    clean)
        # if [ -d leveldb ]; then
        #     (cd leveldb && $MAKE clean)
        # fi
        rm -rf ../priv/*
        rm -rf *.o
        ;;

    test)
        echo $BASEDIR
        export CFLAGS="$CFLAGS -I $BASEDIR/system/include"
        export CXXFLAGS="$CXXFLAGS -I $BASEDIR/system/include"
        export LDFLAGS="$LDFLAGS -L$BASEDIR/system/lib"
        export LD_LIBRARY_PATH="$BASEDIR/system/lib:$LD_LIBRARY_PATH"

        (cd mario && $MAKE check)

        ;;

    get-deps)
        ;;

    *)
        export CFLAGS="$CFLAGS -I $BASEDIR/mario/include"
        export CXXFLAGS="$CXXFLAGS -I $BASEDIR/mario/include"
        export LDFLAGS="$LDFLAGS -L$BASEDIR/mario/lib"
        export LD_LIBRARY_PATH="$BASEDIR/mario/lib:$LD_LIBRARY_PATH"
        # make

        # (cd mario && $MAKE)

        ;;
esac
