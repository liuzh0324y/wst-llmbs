#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

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

class httpclient 
{
public:
    // get loadconf instance
    static httpclient& instance();

    void login();
    
    void logout();

    void reportfile(vector<FileInfo> fileinfo);

    void reportfile(FileInfo fileinfo);

    void reportstatus(uint32_t code, string description);

    static string getToken();

    string timeout();
    int    maxclient();
    int    maxchannel();

    void keeplive();
protected:
    httpclient();
    ~httpclient();

protected:
    static void http_request_get_cb(struct evhttp_request *req, void *arg);
    static void http_request_post_cb(struct evhttp_request *req, void *arg);

    void *http_request_new(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);
    void  http_request_free(struct http_request_get *http_req_get, int req_get_flag);
    int   start_url_request(struct http_request_get *http_req, int req_get_flag);
    void *start_http_request(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);

private:
    static string       _token;
    static string       _time;
    static int          _maxclient;
    static int          _maxchannel;
};

#endif // _HTTP_CLIENT_H_