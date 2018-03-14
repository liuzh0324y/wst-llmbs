bin_PROGRAMS = llmbs

# llmbs_HEADERS = wstapp.h wstrecord.h wstrecordgroup.h wsthttp.h wstconf.h wstlog.h 
llmbs_SOURCES = main.cc wstapp.cc wstrecord.cc wstrecordgroup.cc wsthttp.cc wstconf.cc wstclient.cc wstlog.cc
llmbs_CPPFLAGS = -std=c++11 -I$(LIBEVENT_INCS) -I$(JSONRPC_INCS) -I$(AGORA_INCS)
llmbs_LDADD = -L$(LIBEVENT_LIBS) -L$(JSONRPC_LIBS) -L$(AGORA_LIBS) -ljsoncpp -lrecorder -levent -levent_core -levent_pthreads -lpthread