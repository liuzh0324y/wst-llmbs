#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "wstrecordgroup.h"
#include "wstclient.h"

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

class httpserver
{
public:
    httpserver();
    ~httpserver();

    bool http_server_init();

    bool http_server_run();

    bool http_server_stop();

    void http_server_free();

protected:
    void worker_thread();

    static void request_handler(struct evhttp_request *req, void *arg);
    static void signal_cb(evutil_socket_t sig, short events, void *user_data);

    string get_handler(struct evhttp_request *req, void *arg);
    string post_handler(struct evhttp_request *req, void *arg);

    string parse_json(string jsonstr);
    void   parse_common_json(string jsonstr);
    void   set_common_json(Json::Value &node);
    bool   check_status(string jsonstr);
    
private:
    struct event_base *_base;
    struct evhttp *_http;
    struct evhttp_bound_socket *_handle;
    mutex _mutex;
    JsonHeader _header;
    RecorderGroup _recorderGroup;
};

#endif // _HTTP_SERVER_H_