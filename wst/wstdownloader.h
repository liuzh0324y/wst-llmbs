#ifndef WST_DOWNLOADER_H
#define WST_DOWNLOADER_H

#include "config.h"
#include "IWstHttpEngine.h"
#include <string>

class WstDownloader {
public:
    WstDownloader();
    ~WstDownloader();

    void run(std::string &cid, std::string &url, std::string &time);

protected:
    void worker();

public:
    static void httpRequestGetHandler(struct evhttp_request *req, void *arg);
    static void httpRequestPostHandler(struct evhttp_request *req, void *arg);
    static void httpRequestReadChunk(struct evhttp_request *req, void *param);

    void *httpRequestNew(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);
    void  httpRequestFree(struct http_request_get *http_req_get, int req_get_flag);
    int   startUrlRequest(struct http_request_get *http_req, int req_get_flag);
    void *startHttpRequest(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);

private:
    std::string _cid;
    std::string _url;
    std::string _timestamp;
};

class IWstBlinkHandler {
public:
    ~IWstBlinkHandler() {}

    virtual void onJoin(std::string cid) = 0;
};

class IWstBlinkFactory {
public:
    static IWstBlinkFactory* createWstBlinkFactory(IWstBlinkHandler *eventHandler);

    virtual ~IWstBlinkFactory() {}

    virtual void setSign(std::string sign) = 0;

    virtual void run() = 0;

};

class WstBlinkFactory : public IWstBlinkFactory, IWstHttpEngineEventHandler {
public:
    static WstBlinkFactory* createBlinkFactory(IWstBlinkHandler *eventHandler);

    virtual void setSign(std::string sign);

    virtual void run();

protected:
    virtual void onRequestHandler(struct evhttp_request *req, void *param);
    
private:
    // request sign 
    std::string     _sign;
    IWstHttpEngine *_http = NULL;
};

class WstBlinkHandler : public IWstBlinkHandler {
public:
    WstBlinkHandler();
    virtual ~WstBlinkHandler();

    void run();

protected:
    virtual void onJoin(std::string cid);

private:
    void initialize();

private:
    IWstBlinkFactory *_blink = NULL;
};

#endif // WST_DOWNLOADER_H