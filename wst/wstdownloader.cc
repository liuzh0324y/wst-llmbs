#include "wstdownloader.h"
#include "event.h"
#include "evhttp.h"
#include "wstlog.h"

WstDownloader::WstDownloader() {

}

WstDownloader::~WstDownloader() {

}

void WstDownloader::run(std::string url) {


}

void WstDownloader::httpRequestGetHandler(struct evhttp_request *req, void *arg) {

    struct http_request_get *reuqest = (struct http_request_get*)arg;
    struct evbuffer *buf;
    std::string llbuf;

    buf = evhttp_request_get_input_buffer(req);
    switch (req->response_code)
    {
        case HTTP_OK:
        {
            break;
        }
        default:
            break;
            
    }
    event_base_loopexit(reuqest->base, 0);
}

void WstDownloader::httpRequestPostHandler(struct evhttp_request *req, void *arg) {

    struct http_request_post *http_req_post;
    struct evbuffer *buf;
    string llbuf;

    http_req_post = (struct http_request_post*)arg;

    buf = evhttp_request_get_input_buffer(req);
    switch (req->response_code)
    {
        case HTTP_OK:
        {
            break;
        }
        default:
            break;       
    }

    event_base_loopexit(http_req_post->base, 0);
}

void *WstDownloader::httpRequestNew(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data) {

    int len = 0;
    if (req_get_flag == REQUEST_GET_FLAG)
    {
        len = sizeof(struct http_request_get);
    }
    else if (req_get_flag == REQUEST_POST_FLAG)
    {
        len = sizeof(struct http_request_post);
    }

    struct http_request_get *http_req_get = (struct http_request_get *)calloc(1, len);
    http_req_get->uri = evhttp_uri_parse(url);
    http_req_get->base = base;

    LOGW(data);
    if (req_get_flag == REQUEST_POST_FLAG)
    {
        struct http_request_post *http_req_post = (struct http_request_post*)http_req_get;
        if (content_type == NULL)
        {
            content_type = HTTP_CONTENT_TYPE_URL_ENCODED;
        }
        http_req_post->content_type = strdup(content_type);

        if (data == NULL)
        {
            http_req_post->post_data = NULL;
        }
        else
        {
            http_req_post->post_data = strdup(data);
        }
    }

    return http_req_get;
}

void  WstDownloader::httpRequestFree(struct http_request_get *http_req_get, int req_get_flag) {

    evhttp_connection_free(http_req_get->cn);
    evhttp_uri_free(http_req_get->uri);
    if (req_get_flag == REQUEST_GET_FLAG)
    {
        ::free(http_req_get);
    }
    else if (req_get_flag == REQUEST_POST_FLAG)
    {
        struct http_request_post *http_req_post = (struct http_request_post*)http_req_get;
        if (http_req_post->content_type)
        {
            ::free(http_req_post->content_type);
        }
        if (http_req_post->post_data)
        {
            ::free(http_req_post->post_data);
        }
        ::free(http_req_post);
    }
    http_req_get = NULL;
}

int   WstDownloader::startUrlRequest(struct http_request_get *http_req, int req_get_flag) {

    if (http_req->cn)
    {
        evhttp_connection_free(http_req->cn);
    }
    
    // cout << "((((((((((***************)))))))))))))))" << http_req->uri << endl;
    int port = evhttp_uri_get_port(http_req->uri);

    http_req->cn = evhttp_connection_base_new(http_req->base, NULL, evhttp_uri_get_host(http_req->uri), (port == -1 ? 80 : port));
    
    if (req_get_flag == REQUEST_POST_FLAG)
    {
        http_req->req = evhttp_request_new(httpRequestPostHandler, http_req);
    }
    else if (req_get_flag == REQUEST_GET_FLAG)
    {
        http_req->req = evhttp_request_new(httpRequestGetHandler, http_req);
    }
    
    if (req_get_flag == REQUEST_POST_FLAG)
    {
        const char *path = evhttp_uri_get_path(http_req->uri);
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_POST, path?path:"/");
        struct http_request_post *http_req_post = (struct http_request_post *)http_req;
        evbuffer_add(http_req_post->req->output_buffer, http_req_post->post_data, strlen(http_req_post->post_data));
        evhttp_add_header(http_req_post->req->output_headers, "Content-Type", http_req_post->content_type);
    }
    else if (req_get_flag == REQUEST_GET_FLAG)
    {
        const char *query = evhttp_uri_get_query(http_req->uri);
        const char *path = evhttp_uri_get_path(http_req->uri);
        size_t len = (query ? strlen(query) : 0) + (path ? strlen(path) : 0) + 1;
        char *path_query = NULL;
        if (len > 1)
        {
            path_query = (char*)calloc(len, sizeof(char));
            sprintf(path_query, "%s?%s", path, query);
        }
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_GET, path_query ? path_query : "/");
    }
    evhttp_add_header(http_req->req->output_headers, "Host", evhttp_uri_get_host(http_req->uri));

    return 0;
}

void *WstDownloader::startHttpRequest(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data) {

    struct http_request_get *http_req_get = (struct http_request_get *)httpRequestNew(base, url, req_get_flag, content_type, data);
    
    startUrlRequest(http_req_get, req_get_flag);

    return http_req_get;
}







//===========================class IWstBlinkFactory===========================

IWstBlinkFactory* IWstBlinkFactory::createWstBlinkFactory(IWstBlinkHandler *eventHandler) {
    return WstBlinkFactory::createBlinkFactory(eventHandler);
}

//===========================class WstBlinkFactory===========================

WstBlinkFactory* WstBlinkFactory::createBlinkFactory(IWstBlinkHandler *eventHandler) {
    static WstBlinkFactory theFactory;
    
    return &theFactory;
}

void WstBlinkFactory::onRequestHandler(struct evhttp_request *req, void *param) {
    std::cout << "WstBlinkFactory::onRequestHandler()" << std::endl;
}
void WstBlinkFactory::setSign(std::string sign) {
    _sign = sign;
}

void WstBlinkFactory::run() {
    std::cout << "WstBlinkFactory::run()" << std::endl;
    _http = IWstHttpEngine::createHttpEngine(this);
    std::cout << "WstBlinkFactory::run() --> end" << std::endl;
    try {
        _http->Request("http://glxsslivelocal2.llvision.com:8800/token?appid=1234567890abcdefg&uid=downloader", "", GET);
    } catch (std::exception &ex) {
        std::cout << "what? --> " << ex.what() << std::endl; 
    }
    
}

//===========================class IWstBlinkHandler===========================



//===========================class WstBlinkHandler===========================
WstBlinkHandler::WstBlinkHandler() {
    initialize();
}

WstBlinkHandler::~WstBlinkHandler() {

}

void WstBlinkHandler::initialize() {
    _blink = IWstBlinkFactory::createWstBlinkFactory(this);
}

void WstBlinkHandler::run() {
    _blink->setSign("sign=");
    _blink->run();
}

void WstBlinkHandler::onJoin(std::string cid) {

}