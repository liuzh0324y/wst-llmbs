#!/bin/sh 

SRCDIR=../
OUTDIR=../outdebian

test -d $OUTDIR/DEBIAN                || mkdir -p $OUTDIR/DEBIAN
test -d $OUTDIR/opt/llmbs/bin         || mkdir -p $OUTDIR/opt/llmbs/bin
test -d $OUTDIR/opt/llmbs/conf        || mkdir -p $OUTDIR/opt/llmbs/conf
test -d $OUTDIR/opt/llmbs/tools/agora || mkdir -p $OUTDIR/opt/llmbs/tools/agora
test -d $OUTDIR/opt/llmbs/tools/blink || mkdir -p $OUTDIR/opt/llmbs/tools/blink
test -d $OUTDIR/opt/llmbs/logs        || mkdir -p $OUTDIR/opt/llmbs/logs
test -d $OUTDIR/etc/init.d            || mkdir -p $OUTDIR/etc/init.d
test -d $OUTDIR/etc/profile.d         || mkdir -p $OUTDIR/etc/profile.d
test -d $OUTDIR/etc/ld.so.conf.d      || mkdir -p $OUTDIR/etc/ld.so.conf.d

cp $SRCDIR/debian/control             $OUTDIR/DEBIAN/control
cp $SRCDIR/debian/preinst             $OUTDIR/DEBIAN/preinst
cp $SRCDIR/debian/prerm               $OUTDIR/DEBIAN/prerm
cp $SRCDIR/debian/postinst            $OUTDIR/DEBIAN/postinst
cp $SRCDIR/debian/postrm              $OUTDIR/DEBIAN/postrm

cp $SRCDIR/debian/llmbs.sh            $OUTDIR/etc/profile.d/llmbs.sh
cp $SRCDIR/debian/llmbs               $OUTDIR/etc/init.d/llmbs

cp $SRCDIR/debian/x86_64-linux-llmbs.conf   $OUTDIR/etc/ld.so.conf.d/x86_64-linux-llmbs.conf

cp $SRCDIR/wst/llmbs                  $OUTDIR/opt/llmbs/bin/llmbs

cp $SRCDIR/conf/llmbs.conf            $OUTDIR/opt/llmbs/conf/llmbs.conf
cp $SRCDIR/conf/license.key           $OUTDIR/opt/llmbs/conf/license.key

cp $SRCDIR/tools/agora/ffmpeg               $OUTDIR/opt/llmbs/tools/agora/ffmpeg
cp $SRCDIR/tools/agora/AgoraCoreService     $OUTDIR/opt/llmbs/tools/agora/AgoraCoreService
cp $SRCDIR/tools/agora/convert.py           $OUTDIR/opt/llmbs/tools/agora/convert.py

cp $SRCDIR/tools/blink/videorecorder        $OUTDIR/opt/llmbs/tools/blink/videorecorder
cp -rf $SRCDIR/tools/blink/lib/*            $OUTDIR/opt/llmbs/tools/blink/
 
