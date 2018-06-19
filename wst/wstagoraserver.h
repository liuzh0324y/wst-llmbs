#ifndef WST_AGORA_SERVER_H
#define WST_AGORA_SERVER_H

#include "wstrecordgroup.h"
#include "wstagoraclient.h"

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include <json/json.h>

#include <string.h>

#include <iostream>
#include <string>
#include <mutex>

using std::endl;
using std::cout;
using std::string;
using std::mutex;

class JsonHeader
{
public:
    string      version;
    uint32_t    seqnum;
    string      sessionid;
    string      from;
    string      to;
    string      command;
    string      type;
    string      number;
    string      groupid;
    string      token;

    JsonHeader()
    {
        seqnum = 0;
    }
};

class WstHttpServer
{
public:
    WstHttpServer();
    ~WstHttpServer();

    bool Initialize();

    bool Start();

    bool Stop();

    void Destroy();

protected:
    void workThread();
    void reportMixFileThread(FileInfo info);

    static void requestHandler(struct evhttp_request *req, void *arg);
    static void signalHandler(evutil_socket_t sig, short events, void *user_data);

    string getHandler(struct evhttp_request *req, void *arg);
    string postHandler(struct evhttp_request *req, void *arg);

    string parseJsonRPC(string jsonstr);
    void   parseCommonJsonRPC(string jsonstr);
    void   setCommonJsonRPC(Json::Value &node);
    bool   checkStatus(string jsonstr);
    
private:
    struct event_base *_base;
    struct evhttp *_http;
    struct evhttp_bound_socket *_handle;
    mutex _mutex;
    JsonHeader _header;
    RecorderGroup _recorderGroup;

    void reportThread(FileInfo& info);
};

#endif // WST_AGORA_SERVER_H