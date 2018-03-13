bin_PROGRAMS = llmbs

llmbs_SOURCES = main.cc wstapp.cc wstrecord.cc wstrecordgroup.cc wsthttp.cc wstconf.cc wstclient.cc wstlog.cc
llmbs_CPPFLAGS = -std=c++11 -I$(LIBEVENT_INCS) -I$(JSONRPC_INCS) -I$(AGORA_INCS)
llmbs_LDFLAGS = -L$(LIBEVENT_LIBS) -levent -levent_core -levent_pthreads -L$(JSONRPC_LIBS) -ljsoncpp -L$(AGORA_LIBS) -lrecorder -pthread