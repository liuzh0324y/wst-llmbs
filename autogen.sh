#!/bin/sh
#
# wisdom-media-llmbs autogen.sh
#
# Run this to generate all the initial makefils, etc.
#
# This file has been generated from common

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd "$srcdir"

package=llmbs
srcfile=llmbs.doap

# Make sure we have wst-common
if test ! -f wst-common/wst-autogen.sh;
then
    echo "+ Setting up wst-common submodule"
    git submodule init
fi
git submodule update

# source helper functions
if test ! -f wst-common/wst-autogen.sh;
then
    echo There is something wrong with your source tee.
    echo You are missing wst-common/wst-autogen.sh
    exit 1
fi
. wst-common/wst-autogen.sh

autogen_options $@

printf "+ check for build tools"
if test -z "$NOCHECK"; then
    echo 

    printf "  checking for autoreconf ... "
    echo
    which "autoreconf" 2>/dev/null || {
        echo "not found! Please install the autoconf package."
        exit 1
    }

    printf "  checking for pkg-config ... "
    echo
    which "pkg-config" 2>/dev/null || {
        echo "not found! Please install pkg-config."
        exit 1
    }
else
    echo ": skipped version checks"
fi

# agora
if test ! -f $srcdir/tools/agora/AgoraCoreService;
then
    cp $srcdir/wst-ext/agora/v2.1.0/bin/AgoraCoreService $srcdir/tools/agora/AgoraCoreService
fi

if test ! -f $srcdir/tools/agora/ffmpeg;
then
    cp $srcdir/wst-ext/agora/v2.1.0/tools/ffmpeg $srcdir/tools/agora/ffmpeg
fi

if test ! -f $srcdir/tools/agora/convert.py;
then
    cp $srcdir/wst-ext/agora/convert.py $srcdir/tools/agora/convert.py
fi

# blink
if test ! -f $srcdir/tools/blink/videorecorder;
then
    cp $srcdir/wst-ext/blink/v1.0.0/videorecorder $srcdir/tools/blink/videorecorder
fi

if test ! -f $srcdir/tools/blink/lib;
then
    cp -rf $srcdir/wst-ext/blink/v1.0.0/lib $srcdir/tools/blink/lib
fi

# if no arguments specified then this will be printed
if test -z "$*" && test -z "$NOCONFIGURE"; then
    echo "+ checking for autogen.sh options"
    echo "  This autogen script will automatically run ./configure as:"
    echo "  ./configure $CONFIGURE_DEF_OPT"
    echo "  To pass any additional options, please specify them on the $0"
    echo "  commond line."
fi

toplevel_check $srcfile

# autopoint
if test -d po && grep ^AM_GNU_GETTEXT_VERSION configure.ac >/dev/null; then
    tool_run "autopoint" "--force"
fi

# aclocal
if test -f acinclude.m4; then rm acinclude.m4; fi

autoreconf --force --install || exit 1

test -n "$NOCONFIGURE" && {
    echo "+ skipping configure stage for package $package, as requested."
    echo "+ autogen.sh done."
    exit 0
}

cd "$olddir"

echo "+ running configure"
test ! -z "$CONFIGURE_DEF_OPT" && echo "  default flags: $CONFIGURE_DEF_OPT"
test ! -z "$CONFIGURE_EXT_OPT" && echo "  external flags: $CONFIGURE_EXT_OPT"
echo

echo "$srcdir/configure" $CONFIGURE_DEF_OPT $CONFIGURE_EXT_OPT
"$srcdir/configure" $CONFIGURE_DEF_OPT $CONFIGURE_EXT_OPT || {
    echo "  configure failed"
    exit 1
}

echo "Now type 'make' to compile $package"