ifeq (${CXX},)
CXX=g++
endif
LINK=${CXX}

TOPDIR=`pwd`/3rdparty
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


SRCS=src/main.cc 				\
	src/Application.cc 			\
	src/recorder.cc 			\
	src/loadconf.cc 			\
	src/logger.cc				\
	src/recordergroup.cc 		\
	src/filemerge.cc			\
	src/httpserver.cc 			\
	src/httpclient.cc	

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS) 
	$(LINK) -o $(TARGET) 	\
	$(OBJS) 				\
	$(LDFLAGS) $(LIB)


objs/main.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/main.o 				\
		src/main.cc

objs/Application.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/Application.o 			\
		src/Application.cc 

objs/recorder.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/recorder.o 			\
		src/recorder.cc

objs/loadconf.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/loadconf.o 			\
		src/loadconf.cc

objs/logger.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/logger.o 				\
		src/logger.cc

objs/recordergroup.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/recordergroup.o 		\
		src/recordergroup.cc

objs/filemerge.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/filemerge.o 			\
		src/filemerge.cc 

objs/httpserver.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/httpserver.o 			\
		src/httpserver.cc

objs/httpclient.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/httpclient.o 			\
		src/httpclient.cc

install:
	test -d '/usr/local/llmbs' || mkdir -p '/usr/local/llmbs'
	test -d '/usr/local/llmbs/bin' || mkdir -p '/usr/local/llmbs/bin'
	test -d '/usr/local/llmbs/conf' || mkdir -p '/usr/local/llmbs/conf'
	test -d '/usr/local/llmbs/key' || mkdir -p '/usr/local/llmbs/key'
	test -d '/usr/local/llmbs/logs' || mkdir -p '/usr/local/llmbs/logs'
	test -d '/usr/local/llmbs/tools' || mkdir -p '/usr/local/llmbs/tools'
	
	test ! -f '/usr/local/llmbs/bin/llmbs' || mv '/usr/local/llmbs/bin/llmbs' '/usr/local/llmbs/bin/llmbs.old' 
	cp objs/llmbs '/usr/local/llmbs/bin/llmbs'

	test ! -f '/usr/local/llmbs/conf/llmbs.conf' || mv '/usr/local/llmbs/conf/llmbs.conf' '/usr/local/llmbs/conf/llmbs.conf.default'
	cp conf/llmbs.conf /usr/local/llmbs/conf/llmbs.conf

	test ! -f '/usr/local/llmbs/key/license.key' || mv /usr/local/llmbs/key/license.key /usr/local/llmbs/key/license.key.default
	cp key/license.key /usr/local/llmbs/key/license.key

	test ! -f /usr/local/llmbs/tools/ffmpeg || mv /usr/local/llmbs/tools/ffmpeg /usr/local/llmbs/tools/ffmpeg.old
	cp tools/ffmpeg /usr/local/llmbs/tools/ffmpeg

	test ! -f /usr/local/llmbs/tools/video_recorder || mv /usr/local/llmbs/tools/video_recorder /usr/local/llmbs/tools/video_recorder.old
	cp tools/video_recorder /usr/local/llmbs/tools/video_recorder

	test ! -f /usr/local/llmbs/tools/convert.py || mv /usr/local/llmbs/tools/convert.py /usr/local/llmbs/tools/convert.py.default
	cp tools/convert.py /usr/local/llmbs/tools/convert.py

remove:
	rm -rf /usr/local/llmbs

deb:
	test -d $(OUT)/DEBIAN || mkdir -p $(OUT)/DEBIAN
	test -d $(OUT)/usr/local/llmbs/bin || mkdir -p $(OUT)/usr/local/llmbs/bin
	test -d $(OUT)/usr/local/llmbs/conf || mkdir -p $(OUT)/usr/local/llmbs/conf
	test -d $(OUT)/usr/local/llmbs/logs || mkdir -p $(OUT)/usr/local/llmbs/logs
	test -d $(OUT)/usr/local/llmbs/key || mkdir -p $(OUT)/usr/local/llmbs/key
	test -d $(OUT)/usr/local/llmbs/tools || mkdir -p $(OUT)/usr/local/llmbs/tools
	test -d $(OUT)/etc/init.d || mkdir -p $(OUT)/etc/init.d 
	test -d $(OUT)/etc/profile.d || mkdir -p $(OUT)/etc/profile.d
	test -d $(OUT)/lib/systemd/system || mkdir -p $(OUT)/lib/systemd/system

	cp debian/control $(OUT)/DEBIAN/control
	cp debian/preinst $(OUT)/DEBIAN/preinst
	cp debian/prerm $(OUT)/DEBIAN/prerm
	cp debian/postinst $(OUT)/DEBIAN/postinst
	cp debian/postrm  $(OUT)/DEBIAN/postrm
	cp objs/llmbs $(OUT)/usr/local/llmbs/bin
	cp conf/llmbs.conf $(OUT)/usr/local/llmbs/conf/llmbs.conf
	cp tools/ffmpeg $(OUT)/usr/local/llmbs/tools/ffmpeg
	cp tools/video_recorder $(OUT)/usr/local/llmbs/tools/video_recorder
	cp tools/convert.py $(OUT)/usr/local/llmbs/tools/convert.py
	cp debian/llmbs $(OUT)/etc/init.d/llmbs
	cp debian/llmbs.sh $(OUT)/etc/profile.d/llmbs.sh
	#cp debian/llmbs.service $(OUT)/lib/systemd/system/llmbs.service

	sudo dpkg -b $(OUT) llmbs_$(VERSION)_$(PLATFORM).deb

undeb:
	rm -rf $(OUT)
	rm -rf llmbs_$(VERSION)_$(PLATFORM).deb
