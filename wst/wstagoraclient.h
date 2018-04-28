#ifndef WST_AGORA_CLIENT_H
#define WST_AGORA_CLIENT_H

#include "config.h"

#include <json/json.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <thread>
#include <iostream>
#include <vector>

#include "wstdownloader.h"
#include "wstqueue.h"

using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::vector;


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

    void    ReportStatus(uint32_t code, std::string channelid, std::string username, string description);

    void    GetDownloader();
    
    string  TimeOut();

    int     MaxClient();
    
    int     MaxChannel();

    void    KeepLive();

    void    GetBToken();

    static WstQueue     _wstqueue;
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
    std::string GetNowTime(std::string& starttime, std::string& endtime);

private:
    static string       _token;
    static string       _time;
    static int          _maxclient;
    static int          _maxchannel;
    static string       _btoken;
    
};


#endif // WST_AGORA_CLIENT_H