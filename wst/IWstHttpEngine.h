#ifndef WST_HTTP_CLIENT_H
#define WST_HTTP_CLIENT_H

#include "config.h"

enum HTTPTYPE {
    GET,
    POST,
};

class IWstHttpEngineEventHandler {
public:
    IWstHttpEngineEventHandler() {}
    virtual ~IWstHttpEngineEventHandler() {}

    virtual void onRequestHandler(struct evhttp_request *req, void *param) = 0;
};

class IWstHttpEngine {
public:
    static IWstHttpEngine* createHttpEngine(IWstHttpEngineEventHandler *eventHandler);

    virtual void Request(std::string url, std::string body, HTTPTYPE type) = 0;
};

#endif // WST_HTTP_CLIENT_H