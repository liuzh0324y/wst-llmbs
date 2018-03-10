#!/bin/sh
#
# wisdom-media-llmbs autogen.sh
#
# Run this to generate all the initial makefils, etc.
#
# This file has been generated from common

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

echo $srcdir
olddir=`pwd`
cd "$srcdir"
echo $olddir