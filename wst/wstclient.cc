#include "wstclient.h"
#include "wstconf.h"
#include "wstlog.h"
#include <string>

string       httpclient::_token;
string       httpclient::_time;
int          httpclient::_maxclient = 0;
int          httpclient::_maxchannel = 0;


httpclient::httpclient()
{
    
}

httpclient::~httpclient()
{

}

httpclient& httpclient::Instance()
{
    static httpclient theclient;
    
    return theclient;
}

void httpclient::login()
{
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand
    postjson["command"] = "LOGIN";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number(); // rand
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["ip"] = WstConf::Instance().localip();
    postjson["port"] = string_as_T<int>(WstConf::Instance().localport());
    postjson["username"] = WstConf::Instance().username();
    postjson["password"] = WstConf::Instance().password();
    
    base = event_base_new();

    uri = "http://";
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/auth/login");

    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());

    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

void httpclient::logout()
{
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand
    postjson["command"] = "LOGOUT";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number();
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["token"] = _token;

    base = event_base_new();
    
    uri.append("http://");
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/auth/logout");
    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());
    
    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

void httpclient::reportfile(vector<FileInfo> fileinfo)
{
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand;
    postjson["command"] = "REPORTFILE";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number(); // rand
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["token"] = _token;
    vector<FileInfo>::iterator iter = fileinfo.begin();
    for (iter; iter != fileinfo.end(); iter++)
    {
        postjson["channel"] = (*iter).channel;

        string downurl = "http://";
        downurl.append(WstConf::Instance().localip());
        downurl.append(":");
        downurl.append(WstConf::Instance().httpport());
        downurl.append(WstConf::Instance().httppath());
        string str = (*iter).path.substr(WstConf::Instance().recordpath().length(), (*iter).path.length()-WstConf::Instance().recordpath().length());
        downurl.append(str);
        if (downurl.at(downurl.length()-1) != '/')
        {
            downurl.append("/");
        }
        downurl.append((*iter).name);

        Json::Value file;
        file["name"]=(*iter).name;
        file["meta"]=(*iter).meta;
        file["downurl"]=downurl.c_str();
        file["starttime"]=0;
        file["stoptime"]=0;
        file["thumbnail"]=NULL;
        file["type"]=(*iter).type;
        file["path"]=(*iter).path;
        file["url"]=downurl.c_str();
        postjson["list"].append(file);
    }
    
    
    base = event_base_new();

    uri = "http://";
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/user/reportfile");
    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());
    
    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

void httpclient::reportfile(FileInfo fileinfo)
{
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand;
    postjson["command"] = "REPORTMIXFILE";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number(); // rand
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["token"] = _token;
    postjson["channel"] = fileinfo.channel;
    {
        string downurl = "http://";
        downurl.append(WstConf::Instance().localip());
        downurl.append(":");
        downurl.append(WstConf::Instance().httpport());
        downurl.append(WstConf::Instance().httppath());
        string str = fileinfo.path.substr(WstConf::Instance().recordpath().length(), fileinfo.path.length()-WstConf::Instance().recordpath().length());
        downurl.append(str);
        if (downurl.at(downurl.length()-1) != '/')
        {
            downurl.append("/");
        }
        downurl.append(fileinfo.name);

        string rtmpurl;
        string picurl;

        rtmpurl = "rtmp://";
        rtmpurl.append(WstConf::Instance().localip());
        rtmpurl.append(":");
        rtmpurl.append(WstConf::Instance().rtmpport());
        rtmpurl.append(WstConf::Instance().httppath());
        if (rtmpurl.at(rtmpurl.length()-1) != '/')
        {
            rtmpurl.append("/");
        }
        rtmpurl.append(fileinfo.name);

        picurl = "http://";
        picurl.append(WstConf::Instance().localip());
        picurl.append(":");
        picurl.append(WstConf::Instance().httpport());
        picurl.append(WstConf::Instance().httppath());
        if (picurl.at(picurl.length()-1) != '/')
        {
            picurl.append("/");
        }
        picurl.append(fileinfo.pic);
        

        Json::Value file;
        file["name"]=fileinfo.name;
        file["meta"]=fileinfo.meta;
        file["downurl"]=downurl.c_str();
        file["starttime"]=0;
        file["stoptime"]=0;
        file["thumbnail"]=picurl;
        file["type"]=fileinfo.type;
        file["path"]=fileinfo.path;
        file["url"]=rtmpurl;
        postjson["list"].append(file);
    }
    
    
    base = event_base_new();

    uri = "http://";
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/user/reportfile");
    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());
        
    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

void httpclient::reportstatus(uint32_t code, string description)
{
    if (_token.empty())
    {
        return;
    }
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand
    postjson["command"] = "REPORTSTATUS";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number(); // rand
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["token"] = _token;
    postjson["status"] = code;
    postjson["description"] = description;
    postjson["time"] = time(0);
    base = event_base_new();

    uri = "http://";
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/user/reportstatus");

    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());
        
    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

string httpclient::getToken()
{
    return _token;
}

string httpclient::timeout()
{
    return _time;
}

int    httpclient::maxclient()
{
    return _maxclient;
}

int    httpclient::maxchannel()
{
    return _maxchannel;
}

void httpclient::keeplive()
{
    struct event_base *base;
    struct http_request_post *http_req_post;
    string uri;
    Json::Value postjson;
    postjson["version"] = "V1.0";
    postjson["seqnum"] = WstConf::Instance().seqnum();
    postjson["sessionid"] = "";
    postjson["from"] = WstConf::Instance().number(); // rand
    postjson["to"] = WstConf::Instance().number(); // rand
    postjson["command"] = "KEEPLIVE";
    postjson["type"] = WstConf::Instance().servertype();
    postjson["number"] = WstConf::Instance().number();
    postjson["groupid"] = WstConf::Instance().groupid();
    postjson["token"] = _token;

    base = event_base_new();
    
    uri.append("http://");
    uri.append(WstConf::Instance().serverip());
    uri.append(":");
    uri.append(WstConf::Instance().serverport());
    uri.append("/api/v1/user/keeplive");
    http_req_post = (struct http_request_post *)start_http_request(base,
        uri.c_str(), REQUEST_POST_FLAG, HTTP_CONTENT_TYPE_URL_ENCODED, postjson.toStyledString().c_str());
    
    event_base_dispatch(base);
    http_request_free((struct http_request_get*)http_req_post, REQUEST_POST_FLAG);
    event_base_free(base);
}

void httpclient::http_request_get_cb(struct evhttp_request *req, void *arg)
{
    struct http_request_post *http_req_post = (struct http_request_post*)arg;

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

void httpclient::http_request_post_cb(struct evhttp_request *req, void *arg)
{
    struct http_request_post *http_req_post;
    struct evbuffer *buf;
    string llbuf;

    http_req_post = (struct http_request_post*)arg;

    buf = evhttp_request_get_input_buffer(req);
    switch (req->response_code)
    {
        case HTTP_OK:
        {
            while (evbuffer_get_length(buf))
            {
                int n;
                char cbuf[128];
                n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
                if (n > 0)
                {
                    llbuf.append(cbuf, n);
                }
            }
            LOGW(llbuf);
            Json::Reader jsonreader;
            Json::Value root;
            jsonreader.parse(llbuf.c_str(), root);
            if (!root["command"].isNull())
            {
                string command = root["command"].asString();
                if (command.compare("LOGIN") == 0)
                {
                    _token = root["token"].asString();
                    _time = root["time"].asString();
                    _maxclient = root["maxclient"].asInt();
                    _maxchannel = root["maxchannel"].asInt();
                    cout << "token:" << _token << endl
                         << "time: " << _time << endl
                         << "maxclient: " << _maxclient << endl
                         << "maxchannel: " << _maxchannel << endl;
                }
            }

            if (!root["code"].isNull())
            {
                int32_t code = root["code"].asInt();
                switch (code)
                {
                    case ERROR:
                    {
                        LOGW("code is error!");
                        break;
                    }
                    case SUCCESS:
                    {
                        LOGW("code is success!");
                        break;
                    }
                    case TOKENERROR:
                    {
                        Instance().login();
                        LOGW("code is token error!");
                        break;
                    }
                    case USERPWDERROR:
                    {
                        LOGW("code is userpwd error!");
                        break;
                    }
                    case AUTHFAIL:
                    {
                        LOGW("code is auth failed!");
                        break;    
                    }
                    case NOTDEST:
                    {
                        LOGW("code is not dest!");
                        break;
                    }
                    default:
                    {
                        LOGW("code is unknown!");
                        break;
                    }
                        
                }
            }
            break;
        }
        default:
            LOGW("NO HTTP_OK.");
            break;       
    }

    event_base_loopexit(http_req_post->base, 0);
}

void *httpclient::http_request_new(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data)
{
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

void httpclient::http_request_free(struct http_request_get *http_req_get, int req_get_flag)
{
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

int httpclient::start_url_request(struct http_request_get *http_req, int req_get_flag)
{
    if (http_req->cn)
    {
        evhttp_connection_free(http_req->cn);
    }
    // cout << "((((((((((***************)))))))))))))))" << http_req->uri << endl;
    int port = evhttp_uri_get_port(http_req->uri);
    http_req->cn = evhttp_connection_base_new(http_req->base, NULL, evhttp_uri_get_host(http_req->uri), (port == -1 ? 80 : port));
    
    if (req_get_flag == REQUEST_POST_FLAG)
    {
        http_req->req = evhttp_request_new(http_request_post_cb, http_req);
    }
    else if (req_get_flag == REQUEST_GET_FLAG)
    {
        http_req->req = evhttp_request_new(http_request_get_cb, http_req);
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

void *httpclient::start_http_request(struct event_base *base, const char *url, int req_get_flag, const char *content_type, const char *data)
{
    
    struct http_request_get *http_req_get = (struct http_request_get *)http_request_new(base, url, req_get_flag, content_type, data);
    
    start_url_request(http_req_get, req_get_flag);

    return http_req_get;
}