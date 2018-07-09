#ifndef WST_CLIENT_H
#define WST_CLIENT_H

#include "config.h"

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>

#include <json/json.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <thread>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::vector;

#define HTTP_CONTENT_TYPE_URL_ENCODED   "application/json"
#define HTTP_CONTENT_TYPE_FORM_DATA     "multipart/form-data"
#define HTTP_CONTENT_TYPE_TEXT_PLAIN    "text/plain"

#define REQUEST_POST_FLAG               2
#define REQUEST_GET_FLAG                3

struct http_request_get 
{
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
};

struct http_request_post
{
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
    char *content_type;
    char *post_data;
};

class WstHttpClient 
{
public:
    // get WstConf instance
    static WstHttpClient& Instance();
    static string GetToken();

    void    Login();

    void    Logout();

    void    ReportFile(vector<FileInfo> fileinfo);

    void    ReportFile(FileInfo fileinfo);

    void    ReportStatus(uint32_t code, string description);

    void    ReportFileToGoServer(FileInfo fileinfo);

    string TimeOut();

    int    MaxClient();
    
    int    MaxChannel();

    void KeepLive();
protected:
    WstHttpClient();
    ~WstHttpClient();

protected:
    static void httpRequestGetHandler(struct evhttp_request *req, void *arg);
    static void httpRequestPostHandler(struct evhttp_request *req, void *arg);

    void *httpRequestNew(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);
    void  httpRequestFree(struct http_request_get *http_req_get, int req_get_flag);
    int   startUrlRequest(struct http_request_get *http_req, int req_get_flag);
    void *startHttpRequest(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);

private:
    static string       _token;
    static string       _time;
    static int          _maxclient;
    static int          _maxchannel;
};

#endif // WST_CLIENT_H