#ifndef WST_CONF_H
#define WST_CONF_H

#include "config.h"

/*
 * # note identity
 * TEST= vlaue identity
 */
class WstConf
{
public:
    // get WstConf instance
    static WstConf& Instance();
    // load config file
    bool ReadConfigFile(string file="/opt/llmbs/conf/llmbs.conf");
    // create license
    bool ReadLicenseFile(string file="/opt/llmbs/conf/license.key");

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

    // Blink app path
    void SetBlinkApp(std::string blinkapp);
    std::string GetBlinkApp();

    // Blink app id
    void SetBlinkAppId(std::string appid);
    std::string GetBlinkAppId();

    // Blink channel id
    void SetBlinkChannelId(std::string channelid);
    std::string GetBlinkChannelId();

    // Blink token url
    void SetBlinkTokenUrl(std::string tokenurl);
    std::string GetBlinkTokenUrl();

    // Blink download url
    void SetBlinkDownloadUrl(std::string downloadurl);
    std::string GetBlinkDownloadUrl();

private:
    WstConf();
    ~WstConf();
    // parser config file
    string parseConf(string input);
    string parseConf(vector<string> &input);
    
protected:
    static WstConf     *_instance;
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
    // blink convert tools
    std::string _blinkexe;
    // blink app id
    std::string _blinkappid;
    // blink channel id
    std::string _blinkchannelid;
    // blink token url
    std::string _blinktokenurl;
    // blink downloader url
    std::string _blinkdownloadurl;
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

#endif // WST_CONF_H