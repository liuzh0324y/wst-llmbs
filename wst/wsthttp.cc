#include "wsthttp.h"
#include "wstconf.h"
#include "wstlog.h"

#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif
#include <thread>
using std::thread;


WstHttpServer::WstHttpServer()
{

}

WstHttpServer::~WstHttpServer()
{

}

bool WstHttpServer::Initialize()
{
    return true;
}

bool WstHttpServer::Start()
{
    thread worker(&WstHttpServer::workThread, this);
    worker.join();
    return true;
}

bool WstHttpServer::Stop()
{
	// struct timeval delay = { 2, 0 };

	// printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    // event_base_loopexit(_base, delay);
    // cout << "event_base_loopexit " << endl;
    return true;
}

void WstHttpServer::Destroy()
{

}

bool WstHttpServer::checkStatus(string jsonstr)
{
    Json::Reader reader;
    Json::Value  root;
    
    if (!reader.parse(jsonstr.c_str(), root))
    {
        return false;
    }

    return true;
}

void WstHttpServer::parseCommonJsonRPC(string jsonstr)
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(jsonstr.c_str(), root))
    {
        return;
    }

    if (!root["version"].isNull()) { _header.version = root["version"].asString(); }
    if (!root["seqnum"].isNull()) { _header.seqnum = root["seqnum"].asInt(); }
    if (!root["sessionid"].isNull()) { _header.sessionid = root["sessionid"].asString(); }
    if (!root["from"].isNull()) { _header.from = root["from"].asString(); }
    if (!root["to"].isNull()) { _header.to = root["to"].asString(); }
    if (!root["command"].isNull()) { _header.command = root["command"].asString(); }
    if (!root["type"].isNull()) { _header.type = root["type"].asString(); }
    if (!root["number"].isNull()) { _header.number = root["number"].asString(); }
    if (!root["groupid"].isNull()) { _header.groupid = root["groupid"].asString(); }
    if (!root["token"].isNull()) { _header.token = root["token"].asString(); }
}

void WstHttpServer::setCommonJsonRPC(Json::Value &node)
{
    node["version"] = _header.version;
    node["seqnum"] = _header.seqnum;
    node["sessionid"] = _header.sessionid;
    node["from"] = _header.from;
    node["to"] = _header.to;
    node["command"] = _header.command;
    node["type"] = WstConf::Instance().servertype();
    node["number"] = _header.number;
    node["groupid"] = WstConf::Instance().groupid();
}

string WstHttpServer::parseJsonRPC(string jsonstr)
{
    Json::Reader reader;
    Json::Value root;
    Json::Value result;
    string command;

    parseCommonJsonRPC(jsonstr);
    setCommonJsonRPC(result);
    result["from"] = WstConf::Instance().number(); // rand;
    result["to"] = WstConf::Instance().number(); // rand;
    
    // check
    time_t curtime;
    time(&curtime);
    if (curtime > string_as_T<time_t>(httpclient::Instance().timeout()))
    {
        result["code"] = AUTHORIZATIONDATEEXPIRED;
        return result.toStyledString();
    }

    if (!reader.parse(jsonstr.c_str(), root))
    {
        LOGW("http parse json failed!");
        return string();
    }

    if (!root["command"].isNull())
    {
        command = root["command"].asString();
    }

    if (!command.compare("STARTRECORD"))
    {
        result["command"] = "STARTRECORD";
        string mix = root["mixresolution"].asString();
        if (mix.empty()) mix = "1280,720,15,2400";
        result["code"] = _recorderGroup.start(root["key"].asString(), root["channel"].asString(), mix);
    }
    else if (!command.compare("STOPRECORD"))
    {
        result["command"] = "STOPRECORD";

        result["code"] = _recorderGroup.stop(root["channel"].asString());
    }
    else if (!command.compare("MIXMEDIA"))
    {
        string channel = root["channel"].asString();
        result["command"] = "MIXMEDIA";
        result["code"] = PARAMERROR;
        if (!root["list"].isNull())
        {
            Json::Value node = root["list"];
            if (node.size() == 2)
            {
                vector<FileInfo> fileinfo;
                for (int index = 0; index < node.size(); index++)
                {
                    Json::Value childnode = node[index];
                    FileInfo info;
                    info.name = childnode["name"].asString();
                    info.meta = childnode["meta"].asString();
                    info.type = childnode["type"].asString();
                    info.path = childnode["path"].asString();
                    info.channel = channel;
                    fileinfo.push_back(info);
                }
                result["code"] = _recorderGroup.mixmedia(fileinfo);
            }
            else 
            {
                result["code"] = MIXPARAM2;
            }
        }
    }
    else if (!command.compare("DELETEFILE"))
    {
        result["command"] = "DELETEFILE";
        result["code"] = PARAMERROR;
        if (!root["list"].isNull())
        {
            Json::Value node = root["list"];
            vector<FileInfo> fileinfo;
            for (int index = 0; index < node.size(); index++)
            {
                Json::Value childnode = node[index];
                FileInfo info;
                info.name = childnode["name"].asString();
                info.type = childnode["type"].asString();
                info.path = childnode["path"].asString();
                fileinfo.push_back(info);
            }

            result["code"] = _recorderGroup.deletefile(fileinfo);
        }
    }
    else
    {
        result["command"] = "UNKNOWN";
        result["code"] = ERROR;
    }

    return result.toStyledString();
}

string WstHttpServer::getHandler(struct evhttp_request *req, void *arg)
{
    // struct evbuffer *buf = evbuffer_new();
    // if (!buf)
    // {
    //     cout << "evbuffer_new() failed!" << endl;
    //     return;
    // }

    // evbuffer_add_printf(buf, "get: %s\n", evhttp_request_get_uri(req));
    // evhttp_send_reply(req, HTTP_OK, "OK", buf);
    // evbuffer_free(buf);
    return string();
}

string WstHttpServer::postHandler(struct evhttp_request *req, void *arg)
{
    const char          *uri;
    struct evbuffer     *evbuf;
    struct evhttp_uri   *decoded;
    const char          *path;
    string               inbuf;
    string               outbuf;
    
    uri = evhttp_request_get_uri(req);
    
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        // printf("It's not a good URI. Sending BADREQUEST\n");
        // evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return string();
    }

    /* Let's see what path the user asked for. */
    path = evhttp_uri_get_path(decoded);
    
    if (!path)
    {
        path = "/";
    } 

    evbuf = evhttp_request_get_input_buffer(req);

    while (evbuffer_get_length(evbuf))
    {
        int n;
        char cbuf[128];
        n = evbuffer_remove(evbuf, cbuf, sizeof(cbuf));
        if (n > 0)
        {
            inbuf.append(cbuf, n);
        }
    }

    LOGW(inbuf);
    outbuf = parseJsonRPC(inbuf);
    
    if (decoded)
    {
        evhttp_uri_free(decoded);
    }

    return outbuf;
}

void WstHttpServer::requestHandler(struct evhttp_request *req, void *arg)
{
    struct evbuffer     *buf;
    string               result;
    WstHttpServer          *param;
    char                 len[26];
    struct evkeyvalq    *headers;
    struct evkeyval     *header;
    
    param = (WstHttpServer*)arg;

    switch (evhttp_request_get_command(req))
    {
        case EVHTTP_REQ_GET:
        {
            result = param->getHandler(req, arg);
            break;
        }
        case EVHTTP_REQ_POST:
        {
            result = param->postHandler(req, arg);
            break;
        }
        default:
        {
            break;
        } 
    }  
    
    buf = evbuffer_new();
    sprintf(len, "%d", result.length());
    evhttp_add_header(req->output_headers, "Content-Type", "application/json");
    evhttp_add_header(req->output_headers, "Content-Length", len);
    evbuffer_add_printf(buf, "%s\n", result.c_str());
    if (result.empty())
    {
        evhttp_send_reply(req, HTTP_NOTFOUND, "NOTFOUND", buf);
    }
    else
    {
        evhttp_send_reply(req, HTTP_OK, "OK", buf);
    }
    // evbuffer * out = evhttp_request_get_output_buffer(req);
    string outlog("send json: ");
    outlog.append(result);
    LOGW(outlog);
    if (buf)
    {
        evbuffer_free(buf);
    }  
}

void WstHttpServer::signalHandler(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = { 2, 0 };

	event_base_loopexit(base, &delay);
}

void WstHttpServer::workThread()
{
    struct event *signal_event;

    LOGW("login start");
    httpclient::Instance().login();
    LOGW("login end");
    
    _base = event_base_new();
    if (!_base)
    {
        LOGW("couldn't create an event_base");
        return;
    }

    _http = evhttp_new(_base);
    if (!_http)
    {
        LOGW("couldn't create evhttp.");
        return;
    }

    evhttp_set_gencb(_http, requestHandler, this);

    _handle = evhttp_bind_socket_with_handle(_http, "0.0.0.0", stoi(WstConf::Instance().localport()));
    if (!_handle)
    {
        string log("couldn't bind to port: ");
        log.append(WstConf::Instance().localport());
        LOGW(log);
        return;
    }

    signal_event = evsignal_new(_base, SIGINT, signalHandler, (void *)_base);

    struct sockaddr_storage ss;
    evutil_socket_t fd;
    ev_socklen_t socklen = sizeof(ss);
    char addrbuf[128];
    void *inaddr;
    const char *addr;
    int got_port = -1;
    char uri_root[512];
    fd = evhttp_bound_socket_get_fd(_handle);
    memset(&ss, 0, sizeof(ss));
    if (getsockname(fd, (struct sockaddr *)&ss, &socklen))
    {
        LOGW("getsockname() failed.");
        return;
    }
    if (ss.ss_family == AF_INET)
    {
        got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
        inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
    }
    else if (ss.ss_family == AF_INET6)
    {
        got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
        inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
    }
    else
    {
        LOGW("Weird address family ");
        return;
    }

    addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
    if (addr)
    {
        char out[256];
        memset(out, 0, 256);
        sprintf(out, "listening on %s:%d", addr, got_port);
        LOGW(out);
        evutil_snprintf(uri_root, sizeof(uri_root), "http://%s:%d", addr, got_port);
    }
    else
    {
        LOGW("evutil_inet_ntop failed");
    }

    event_base_dispatch(_base);
    if (_http)
    {
        evhttp_free(_http);
        _http = NULL;
    }
    if (_base)
    {
        event_base_free(_base);
        _base = NULL;
    }
    httpclient::Instance().logout();
    LOGW("worker_thread end");
}