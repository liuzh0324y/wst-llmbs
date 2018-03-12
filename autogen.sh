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

# Make sure we have wst-common
if test ! -f wst-common/wst-autogen.sh;
then
    echo "+ Setting up wst-common submodule"
    git submodule init
fi
git submodule update --remote

# source helper functions
if test ! -f wst-common/wst-autogen.sh;
then
    echo There is something wrong with your source tee.
    echo You are missing wst-common/wst-autogen.sh
    exit 1
fi
. wst-common/wst-autogen.sh

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