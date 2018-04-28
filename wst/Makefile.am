bin_PROGRAMS = llmbs
# llmbs_HEADERS = wstapp.h wstrecord.h wstrecordgroup.h wsthttp.h wstconf.h wstlog.h 
llmbs_SOURCES =  wstqueue.cc main.cc wstapp.cc wstrecord.cc wstrecordgroup.cc wstagoraserver.cc wstconf.cc wstagoraclient.cc wstlog.cc wstblinkrecord.cc wstdownloader.cc IWstHttpEngine.cc WstHttpEngine.cc
llmbs_CPPFLAGS = -std=c++11 -I$(LIBEVENT_INCS) -I$(JSONRPC_INCS) -I$(AGORA_INCS)
llmbs_LDADD = -L$(LIBEVENT_LIBS) -L$(JSONRPC_LIBS) -L$(AGORA_LIBS) -ljsoncpp -lrecorder -levent -levent_core -levent_pthreads -lpthread