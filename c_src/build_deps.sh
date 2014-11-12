#!/bin/sh

MARIO_VSN="m1.0.6"

# move the path th c_src
if [ `basename $PWD` != "c_src" ]; then
    cd c_src
fi

BASEDIR="$PWD"

case "$1" in
    rm-deps)
        rm -rf mario
        ;;

    clean)
        rm -rf ../priv/
        rm -rf ../ebin/
        rm -rf *.o
        rm -rf mario
        rm -rf ../log/*
        rm -rf ../log

        ;;

    test)
        echo $BASEDIR
        export CFLAGS="$CFLAGS -I $BASEDIR/system/include"
        export CXXFLAGS="$CXXFLAGS -I $BASEDIR/system/include"
        export LDFLAGS="$LDFLAGS -L$BASEDIR/system/lib"
        export LD_LIBRARY_PATH="$BASEDIR/system/lib:$LD_LIBRARY_PATH"

        (cd mario && make check)

        ;;

    get-deps)
        if [ ! -d mario ]; then
            git clone git://github.com/Qihoo360/Mario.git ./mario
            (cd mario && git checkout $MARIO_VSN)
        fi
        ;;

    *)
        export CFLAGS="$CFLAGS -I $BASEDIR/mario/include"
        export CXXFLAGS="$CXXFLAGS -I $BASEDIR/mario/include"
        export LDFLAGS="$LDFLAGS -L$BASEDIR/mario/lib"
        export LD_LIBRARY_PATH="$BASEDIR/mario/lib:$LD_LIBRARY_PATH"

        if [ ! -d mario ]; then
            git clone git://github.com/Qihoo360/Mario.git ./mario
            (cd mario && git checkout $MARIO_VSN)
        fi
        (cd mario && make)
        mkdir -p ../log

        ;;
esac
