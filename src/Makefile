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
	test -d '/usr/local/llmbs/conf' || mkdir -p '/usr/local/llmbs/conf'
	test -d '/usr/local/llmbs/key' || mkdir -p '/usr/local/llmbs/key'
	test -d '/usr/local/llmbs/logs' || mkdir -p '/usr/local/llmbs/logs'
	test -d '/usr/local/llmbs/tools' || mkdir -p '/usr/local/llmbs/tools'
	
	test ! -f '/usr/local/bin/llmbs' || mv '/usr/local/bin/llmbs' '/usr/local/bin/llmbs.old' 
	cp objs/llmbs '/usr/local/bin/llmbs'

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
	rm -rf /usr/local/bin/llmbs*
	rm -rf /usr/local/llmbs

deb:
	test -d deb/DEBIAN || mkdir -p deb/DEBIAN
	test -d deb/usr/local/bin || mkdir -p deb/usr/local/bin
	test -d deb/usr/local/llmbs/conf || mkdir -p deb/usr/local/llmbs/conf
	test -d deb/usr/local/llmbs/logs || mkdir -p deb/usr/local/llmbs/logs
	test -d deb/usr/local/llmbs/key || mkdir -p deb/usr/local/llmbs/key
	test -d deb/usr/local/llmbs/tools || mkdir -p deb/usr/local/llmbs/tools
	test -d deb/etc/init.d || mkdir -p deb/etc/init.d 
	test -d deb/lib/systemd/system || mkdir -p deb/lib/systemd/system

	cp debian/control deb/DEBIAN/control
	cp debian/preinst deb/DEBIAN/preinst
	cp debian/prerm deb/DEBIAN/prerm
	cp debian/postinst deb/DEBIAN/postinst
	cp debian/postrm  deb/DEBIAN/postrm
	cp objs/llmbs deb/usr/local/bin
	cp conf/llmbs.conf deb/usr/local/llmbs/conf/llmbs.conf
	cp tools/ffmpeg deb/usr/local/llmbs/tools/ffmpeg
	cp tools/video_recorder deb/usr/local/llmbs/tools/video_recorder
	cp tools/convert.py deb/usr/local/llmbs/tools/convert.py
	cp debian/llmbs deb/etc/init.d/llmbs
	#cp debian/llmbs.service deb/lib/systemd/system/llmbs.service

	sudo dpkg -b deb llmbs_1.0.1_linux-amd64.deb

undeb:
	rm -rf deb
	rm -rf llmbs_1.0.1_linux-amd64.deb