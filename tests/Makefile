ifeq (${CXX},)
CXX=g++
endif
LINK=${CXX}

LDFLAGS=-static-libgcc -std=c++11
CXXFLAGS=-pipe -std=c++11 -fPIC -g -fno-omit-frame-pointer \
			-DNDEBUG=1 -Wconversion -O3 -Wall -W -fvisibility=hidden
					
LIB=-pthread
INCPATH=-I.
TARGET=bin/server

OBJS=objs/tests/main.o

SRCS=test/main.cc

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS) 
	$(LINK) -o $(TARGET) 	\
	$(OBJS) 				\
	$(LDFLAGS) $(LIB)

objs/tests/main.o:	
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objs/tests/main.o 				\
		tests/main.cc

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)