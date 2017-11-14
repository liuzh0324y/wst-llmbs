#ifndef _LOAD_CONF_H_
#define _LOAD_CONF_H_

#include "config.h"

/*
 * # note identity
 * TEST= vlaue identity
 */
class loadconf
{
public:
    // get loadconf instance
    static loadconf& instance();
    // load config file
    bool load_config_file(string file="/usr/local/llmbs/conf/llmbs.conf");
    // create license
    bool load_license_file(string file="/usr/local/llmbs/key/license.key");

    // user name
    string username();
    void   setusername(const string& name);
    // user password
    string password();
    void   setpassword(const string& password);
    // local ip
    string localip();
    void   setlocalip(const string& ip);
    // local port
    string localport();
    void   setlocalport(const string& port);
    // server ip
    string serverip();
    void   setserverip(const string& ip);
    // server port
    string serverport();
    void   setserverport(const string& port);
    // connect timeout
    uint32_t connecttimeout();
    void   setconnectimeout(const uint32_t& timeout);
    // send timeout
    uint32_t sendtimeout();
    void   setsendtimeout(const uint32_t& timeout);
    // recv timeout
    uint32_t recvtimeout();
    void   setrecvtimeout(const uint32_t& timeout);
    // record path
    string recordpath();
    void   setrecordpath(const string& path);
    // logs path
    string logspath();
    void   setlogspath(const string& path);
    // applite path
    string applitepath();
    void   setapplitepath(const string& path);
    // group id
    string groupid();
    void   setgroupid(const string& id);
    // server type
    string servertype();
    void   setservertype(const string& type);
    // http path
    string httppath();
    void   sethttppath(const string& path);
    // http port
    string httpport();
    void   sethttpport(const string& port);
    // rtmp port
    string rtmpport();
    void   setrtmpport(const string& port);
    // free disk
    uint32_t freedisk();
    void     setfreedisk(const int size);
    // keep live
    uint32_t keeplivetime();
    void     setkeeplivetime(const int timeout);
    // idle limit second
    uint32_t idleLimitSec();
    void     setIdleLimitSec(const uint32_t sec);
    // get number
    string number();
    // get seq number
    uint32_t seqnum();

private:
    loadconf();
    ~loadconf();
    // parser config file
    string _parse_conf(string input);
    string _parse_conf(vector<string> &input);
    
protected:
    static loadconf     *_instance;
    static mutex         _lock;
    ifstream             _input;
    uint32_t             _seqnum;

private:
    // user name
    string     _username;
    // user password
    string     _password;
    // local ip
    string     _localip;
    // local port
    string     _localport;
    // server ip
    string     _serverIp;
    // server port
    string     _serverPort;
    // connect server timeout
    uint32_t   _connectTimeout;
    // socket send timeout
    uint32_t   _sendTimeout;
    // socket recv timeout
    uint32_t   _recvTimeout;
    // record file path
    string     _recordpath;
    // application logs path
    string     _logspath;
    // applite path
    string     _applitepath;
    // group id
    string     _groupid;
    // server type
    string     _type;
    // http path
    string     _httppath;
    // http port
    string     _httpport;
    // rtmp port
    string     _rtmpport;
    // freedisk
    uint32_t   _freedisk;
    // keep live
    uint32_t   _keeplive;
    // idle limit second
    uint32_t   _idleLimitSec;
};

template<class T>
T string_as_T(const std::string& s)
{
    T t;
    std::istringstream ist(s);
    ist >> t;
    return t;
}

template<class T>
std::string T_as_string(const T& t)
{
    std::ostringstream ost;
    ost << t;
    return ost.str();
}

#endif // _LOAD_CONF_H_