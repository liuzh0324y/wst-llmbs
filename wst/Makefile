ifeq (${CXX},)
CXX=g++
endif
LINK=${CXX}

TOPDIR=`pwd`/wst-ext
LIBPATH=${TOPDIR}/agora/v1.3.0/libs 
EVEPATH=${TOPDIR}/libevent/lib
JSONPATH=${TOPDIR}/json/lib
LDFLAGS=-static-libgcc -std=c++11
CXXFLAGS=-pipe -std=c++11 -fPIC -g -fno-omit-frame-pointer \
			-DNDEBUG=1 -DLOGER=1 -Wconversion -O3 -Wall -W -fvisibility=hidden
					
# -levent_core -levent_pthreads
LIB=-pthread -lrt -L$(LIBPATH) -lRecordEngine -L$(EVEPATH) -levent -L$(JSONPATH) -ljsoncpp 
INCPATH=-I. -I${TOPDIR}/agora/v1.3.0/include -I${TOPDIR}/libevent/include -I${TOPDIR}/json/include
TARGET=objs/llmbs
OUT=out
VERSION=1.0.1
PLATFORM=xenial-amd64

OBJS=objs/main.o 				\
	objs/Application.o 			\
	objs/recorder.o 			\
	objs/loadconf.o 			\
	objs/logger.o				\
	objs/recordergroup.o 		\
	objs/filemerge.o			\
	objs/httpserver.o			\
	objs/httpclient.o


SRCS=wst/main.cc 				\
	wst/Application.cc 			\
	wst/recorder.cc 			\
	wst/loadconf.cc 			\
	wst/logger.cc				\
	wst/recordergroup.cc 		\
	wst/filemerge.cc			\
	wst/httpserver.cc 			\
	wst/httpclient.cc	

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS) 
	$(LINK) -o $(TARGET) 	\
	$(OBJS) 				\
	$(LDFLAGS) $(LIB)


objs/main.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/main.o 				\
		wst/main.cc

objs/Application.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/Application.o 			\
		wst/Application.cc 

objs/recorder.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/recorder.o 			\
		wst/recorder.cc

objs/loadconf.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/loadconf.o 			\
		wst/loadconf.cc

objs/logger.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/logger.o 				\
		wst/logger.cc

objs/recordergroup.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/recordergroup.o 		\
		wst/recordergroup.cc

objs/filemerge.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/filemerge.o 			\
		wst/filemerge.cc 

objs/httpserver.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/httpserver.o 			\
		wst/httpserver.cc

objs/httpclient.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/httpclient.o 			\
		wst/httpclient.cc

install:
	test -d '/opt/llmbs' || mkdir -p '/opt/llmbs'
	test -d '/opt/llmbs/bin' || mkdir -p '/opt/llmbs/bin'
	test -d '/opt/llmbs/conf' || mkdir -p '/opt/llmbs/conf'
	test -d '/opt/llmbs/key' || mkdir -p '/opt/llmbs/key'
	test -d '/opt/llmbs/logs' || mkdir -p '/opt/llmbs/logs'
	test -d '/opt/llmbs/tools' || mkdir -p '/opt/llmbs/tools'
	
	test ! -f '/opt/llmbs/bin/llmbs' || mv '/opt/llmbs/bin/llmbs' '/opt/llmbs/bin/llmbs.old' 
	cp objs/llmbs '/opt/llmbs/bin/llmbs'

	test ! -f '/opt/llmbs/conf/llmbs.conf' || mv '/opt/llmbs/conf/llmbs.conf' '/opt/llmbs/conf/llmbs.conf.default'
	cp conf/llmbs.conf /opt/llmbs/conf/llmbs.conf

	test ! -f '/opt/llmbs/key/license.key' || mv /opt/llmbs/key/license.key /opt/llmbs/key/license.key.default
	cp key/license.key /opt/llmbs/key/license.key

	test ! -f /opt/llmbs/tools/ffmpeg || mv /opt/llmbs/tools/ffmpeg /opt/llmbs/tools/ffmpeg.old
	cp tools/ffmpeg /opt/llmbs/tools/ffmpeg

	test ! -f /opt/llmbs/tools/video_recorder || mv /opt/llmbs/tools/video_recorder /opt/llmbs/tools/video_recorder.old
	cp tools/video_recorder /opt/llmbs/tools/video_recorder

	test ! -f /opt/llmbs/tools/convert.py || mv /opt/llmbs/tools/convert.py /opt/llmbs/tools/convert.py.default
	cp tools/convert.py /opt/llmbs/tools/convert.py

remove:
	rm -rf /opt/llmbs

deb:
	test -d $(OUT)/DEBIAN || mkdir -p $(OUT)/DEBIAN
	test -d $(OUT)/opt/llmbs/bin || mkdir -p $(OUT)/opt/llmbs/bin
	test -d $(OUT)/opt/llmbs/conf || mkdir -p $(OUT)/opt/llmbs/conf
	test -d $(OUT)/opt/llmbs/logs || mkdir -p $(OUT)/opt/llmbs/logs
	test -d $(OUT)/opt/llmbs/key || mkdir -p $(OUT)/opt/llmbs/key
	test -d $(OUT)/opt/llmbs/tools || mkdir -p $(OUT)/opt/llmbs/tools
	test -d $(OUT)/etc/init.d || mkdir -p $(OUT)/etc/init.d 
	test -d $(OUT)/etc/profile.d || mkdir -p $(OUT)/etc/profile.d
	test -d $(OUT)/lib/systemd/system || mkdir -p $(OUT)/lib/systemd/system

	cp debian/control $(OUT)/DEBIAN/control
	cp debian/preinst $(OUT)/DEBIAN/preinst
	cp debian/prerm $(OUT)/DEBIAN/prerm
	cp debian/postinst $(OUT)/DEBIAN/postinst
	cp debian/postrm  $(OUT)/DEBIAN/postrm
	cp objs/llmbs $(OUT)/opt/llmbs/bin
	cp conf/llmbs.conf $(OUT)/opt/llmbs/conf/llmbs.conf
	cp tools/ffmpeg $(OUT)/opt/llmbs/tools/ffmpeg
	cp tools/video_recorder $(OUT)/opt/llmbs/tools/video_recorder
	cp tools/convert.py $(OUT)/opt/llmbs/tools/convert.py
	cp debian/llmbs $(OUT)/etc/init.d/llmbs
	cp debian/llmbs.sh $(OUT)/etc/profile.d/llmbs.sh
	#cp debian/llmbs.service $(OUT)/lib/systemd/system/llmbs.service

	sudo dpkg -b $(OUT) llmbs_$(VERSION)_$(PLATFORM).deb

undeb:
	rm -rf $(OUT)
	rm -rf llmbs_$(VERSION)_$(PLATFORM).deb
