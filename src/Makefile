ifeq (${CXX},)
CXX=g++
endif
LINK=${CXX}

TOPDIR=`pwd`/3rdparty
LIBPATH=${TOPDIR}/agora/libs 
EVEPATH=${TOPDIR}/libevent/lib
JSONPATH=${TOPDIR}/json/lib
LDFLAGS=-static-libgcc -std=c++11
CXXFLAGS=-pipe -std=c++11 -fPIC -g -fno-omit-frame-pointer \
			-DNDEBUG=1 -DLOGER=1 -Wconversion -O3 -Wall -W -fvisibility=hidden
					
# -levent_core -levent_pthreads
LIB=-pthread -lrt -L$(LIBPATH) -lRecordEngine -L$(EVEPATH) -levent -L$(JSONPATH) -ljsoncpp 
INCPATH=-I. -I${TOPDIR}/agora/include -I${TOPDIR}/libevent/include -I${TOPDIR}/json/include
TARGET=bin/llmbs

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

clean:
	-rm -f $(TARGET)
	-rm -f $(OBJS)
