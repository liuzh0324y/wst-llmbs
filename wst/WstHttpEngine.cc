#include "WstHttpEngine.h"

WstHttpEngine* WstHttpEngine::CreateHttpEngine(IWstHttpEngineEventHandler * eventHandler) {
    std::cout << "WstHttpEngine::createHttpEngine()" << std::endl;
    static WstHttpEngine theEngine;
    theEngine._eventHandler = eventHandler;
    return &theEngine;
}


void WstHttpEngine::Request(std::string url, std::string body, HTTPTYPE type) {
    std::cout << "WstHttpEngine::Request()" << std::endl;
    struct event_base *base;
    struct http_request_get *request;
    int flag = REQUEST_GET_FLAG;
    if (type == POST) {
        flag = REQUEST_POST_FLAG;
    }
    base = event_base_new();
    request = (struct http_request_get *)startHttpRequest(base, url.c_str(), flag, HTTP_CONTENT_TYPE_URL_ENCODED, body.c_str());

    event_base_dispatch(base);
    httpRequestFree((struct http_request_get*)request, flag);
    event_base_free(base);
}

void WstHttpEngine::httpRequestGetHandler(struct evhttp_request *req, void *arg) {
    struct http_request_get *reuqest = (struct http_request_get*)arg;
    WstHttpEngine * param = (WstHttpEngine*)arg;
    if (param->_eventHandler) {
        param->_eventHandler->onRequestHandler(req, NULL);
        event_base_loopexit(reuqest->base, 0);
        return;
    }
    
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

void WstHttpEngine::httpRequestPostHandler(struct evhttp_request *req, void *arg) {
    struct http_request_post *http_req_post = (struct http_request_post*)arg;
    WstHttpEngine * param = (WstHttpEngine *)http_req_post->param;
    if (param->_eventHandler) {
        param->_eventHandler->onRequestHandler(req, arg);
        event_base_loopexit(http_req_post->base, 0);
        return;
    }

    struct evbuffer *buf;
    string llbuf;

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

void *WstHttpEngine::httpRequestNew(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data) {
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
    http_req_get->param = this;

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

void  WstHttpEngine::httpRequestFree(struct http_request_get *http_req_get, int req_get_flag) {
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

int   WstHttpEngine::startUrlRequest(struct http_request_get *http_req, int req_get_flag) {
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

void *WstHttpEngine::startHttpRequest(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data) {
    struct http_request_get *http_req_get = (struct http_request_get *)httpRequestNew(base, url, req_get_flag, content_type, data);
    
    startUrlRequest(http_req_get, req_get_flag);

    return http_req_get;
}