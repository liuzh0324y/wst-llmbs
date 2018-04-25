#ifndef WST_HTTP_ENGINE_H
#define WST_HTTP_ENGINE_H

#include "config.h"
#include "IWstHttpEngine.h"

class WstHttpEngine : public IWstHttpEngine {
public:
    static WstHttpEngine* CreateHttpEngine(IWstHttpEngineEventHandler * eventHandler);

    virtual void Request(std::string url, std::string body, HTTPTYPE type);
    
    static void httpRequestGetHandler(struct evhttp_request *req, void *arg);
    static void httpRequestPostHandler(struct evhttp_request *req, void *arg);

    void *httpRequestNew(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);
    void  httpRequestFree(struct http_request_get *http_req_get, int req_get_flag);
    int   startUrlRequest(struct http_request_get *http_req, int req_get_flag);
    void *startHttpRequest(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data);

private:
    IWstHttpEngineEventHandler * _eventHandler = NULL;
};

#endif // WST_HTTP_ENGINE_H