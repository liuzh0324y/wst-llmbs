#include "wstconf.h"
#include "wstlog.h"

WstConf *WstConf::_instance = NULL;
mutex WstConf::_lock;

WstConf::WstConf()
{
    _seqnum = 0;
    _freedisk = 300;
    _keeplive = 3600;
    _idleLimitSec = 3600;
    
    _username = "admin";
    _password = "123456";
    _localip = "127.0.0.1";
    _localport = "18005";
    _serverIp = "127.0.0.1";
    _serverPort = "18003";
    _connectTimeout = 30000;
    _sendTimeout = 30000;
    _recvTimeout = 30000;
    _recordpath = "/record";
    _logspath = "/opt/llmbs/logs";
    _applitepath = "/opt/llmbs/tools";
    _groupid = "0";
    _type = "ambs";
    _httppath = "/record";
    _httpport = "18020";
    _rtmpport = "18019";
    _freedisk = 300;
    _keeplive = 3600;
    _idleLimitSec = 3600;

    // Blink
    _blinkexe = "/opt/blink/videorecorder";
    _blinkappid = "1234567890abcdefg";
    _blinkchannelid = "downloader";
    _blinktokenurl = "http://glxsslivelocal2.llvision.com:8800/token";
    _blinkdownloadurl = "http://glxsslivelocal2.llvision.com:8040/downloadurl";
}

WstConf::~WstConf()
{

}

WstConf& WstConf::Instance()
{    
    static WstConf theloadconf;
    return theloadconf;
}

bool WstConf::ReadConfigFile(string file)
{
    vector<string> filevec;
    _input.open(file);
    if (!_input)
    {
        std::stringstream temp;
        temp << "could not load " << file;
        LOGW(temp.str());
        return false;
    }

    while (!_input.eof())
    {
        char buffer[256];
        memset(buffer, 0, 256);
        // _input.getline(buffer, 256);
        _input.getline(buffer, 256, '\n');
        if ((strlen(buffer)==0) || (buffer[0] == '#'))
        {
            continue;
        }
        filevec.push_back(buffer);
        // cout << buffer << ", size: " << strlen(buffer) << endl;
        // parseConf(string(buffer));
    }

    _input.close();

    parseConf(filevec);
    // LOGW("load config file.");
    return true;
}

bool WstConf::ReadLicenseFile(string file)
{
    ofstream fs;
    try
    {
        fs.open(file);
        
        fs << "key: " << number();
        fs.close();
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << endl;
    }
    
    // LOGW("load license file.");
    return true;
}

string WstConf::parseConf(vector<string> &filelist)
{
    vector<string>::iterator iter = filelist.begin();
    for (iter; iter != filelist.end(); iter++)
    {
        string input = (*iter);
        if (std::size_t pos = input.find("="))
        {
            string temp = input.substr(0, pos);
            string tempstr = input.substr(pos+1);
    
            if (!temp.compare("username"))
            {
                _username = tempstr;
            }
            else if (temp.compare("password") == 0)
            {
                _password = tempstr;
            }
            else if (temp.compare("localip") == 0)
            {
                _localip = tempstr;
            }
            else if (temp.compare("localport") == 0)
            {
                _localport = tempstr;
            }
            else if (temp.compare("serverip") == 0)
            {
                _serverIp = tempstr;
            }
            else if (temp.compare("serverport") == 0)
            {
                _serverPort = tempstr;
            }
            else if (temp.compare("connecttimeout") == 0)
            {
                _connectTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("sendtimeout") == 0)
            {
                _sendTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("recvtimeout") == 0)
            {
                _recvTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("recordpath") == 0)
            {
                _recordpath = tempstr;
            }
            // else if (temp.compare("logspath") == 0)
            // {
            //     _logspath = tempstr;
            // }
            // else if (temp.compare("applitepath") == 0)
            // {
            //     _applitepath = tempstr;
            // }
            else if (temp.compare("groupid") == 0)
            {
                _groupid = tempstr;
            }
            else if (temp.compare("type") == 0)
            {
                _type = tempstr;
            }
            else if (temp.compare("httppath") == 0)
            {
                _httppath = tempstr;
            }
            else if (temp.compare("httpport") == 0)
            {
                _httpport = tempstr;
            }
            else if (temp.compare("rtmpport") == 0)
            {
                _rtmpport = tempstr;
            }
            else if (temp.compare("freedisk") == 0)
            {
                _freedisk = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("keeplive") == 0)
            {
                _keeplive = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("idleLimitSec") == 0)
            {
                _idleLimitSec = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("blinkapp") == 0) {
                _blinkexe = tempstr;
            }
            else if (temp.compare("blinkappid") == 0) {
                _blinkappid = tempstr;
            }
            else if (temp.compare("blinkchannelid") == 0) {
                _blinkchannelid = tempstr;
            }
            else if (temp.compare("blinktokenurl") == 0) {
                _blinktokenurl = tempstr;
            }
            else if (temp.compare("blinkdownloadurl") == 0) {
                _blinkdownloadurl = tempstr;
            }
        }
    } 
    return string("parse conf");
}

unsigned int veax;
unsigned int vebx;
unsigned int vecx;
unsigned int vedx;

void cpuid(unsigned int in)
{
    asm("cpuid":
        "=a"(veax),
        "=b"(vebx),
        "=c"(vecx),
        "=d"(vedx):
        "a"(in));
}

void LM(unsigned int var, uint32_t *vx)
{
    for (int i = 0; i < 3; i++)
    {
        var = (var>>i);
        vx[i] = var;
    }
}

static void getcpuid(char *id)
{
    uint32_t ax[3],cx[3],dx[3];
    cpuid(1);
    LM(veax, ax);
    cpuid(3);
    LM(vecx, cx);
    LM(vedx, dx);
    sprintf(id, "%u%u%u%u%u%u%u%u%u", ax[0], ax[1], ax[2],
                                      cx[0], cx[1], cx[2],
                                      dx[0], dx[1], dx[2]);
}

string WstConf::number()
{
    char cpuid[100];
    getcpuid(cpuid);

    return cpuid;
}

uint32_t WstConf::seqnum()
{
    return _seqnum++;
}

// user name
string WstConf::username()
{
    return _username;
}

void   WstConf::setusername(const string& name)
{
    _username = name;
}


// user password
string WstConf::password()
{
    return _password;
}

void   WstConf::setpassword(const string& password)
{
    _password = password;
}


// local ip
string WstConf::localip()
{
    return _localip;
}

void   WstConf::setlocalip(const string& ip)
{
    _localip = ip;
}


// local port
string WstConf::localport()
{
    return _localport;
}

void   WstConf::setlocalport(const string& port)
{
    _localport = port;
}


// server ip
string WstConf::serverip()
{
    return _serverIp;
}

void   WstConf::setserverip(const string& ip)
{
    _serverIp = ip;
}


// server port
string WstConf::serverport()
{
    return _serverPort;
}

void   WstConf::setserverport(const string& port)
{
    _serverPort = port;
}


// connect timeout
uint32_t WstConf::connecttimeout()
{
    return _connectTimeout;
}

void   WstConf::setconnectimeout(const uint32_t& timeout)
{
    _connectTimeout = timeout;
}


// send timeout
uint32_t WstConf::sendtimeout()
{
    return _sendTimeout;
}

void   WstConf::setsendtimeout(const uint32_t& timeout)
{
    _sendTimeout = timeout;
}


// recv timeout
uint32_t WstConf::recvtimeout()
{
    return _recvTimeout;
}

void   WstConf::setrecvtimeout(const uint32_t& timeout)
{
    _recvTimeout = timeout;
}


// record path
string WstConf::recordpath()
{
    return _recordpath;
}

void   WstConf::setrecordpath(const string& path)
{
    _recordpath = path;
}


// logs path
string WstConf::logspath()
{
    return _logspath;
}

void   WstConf::setlogspath(const string& path)
{
    _logspath = path;
}


// applite path
string WstConf::applitepath()
{
    return _applitepath;
}

void   WstConf::setapplitepath(const string& path)
{
    _applitepath = path;
}


// group id
string WstConf::groupid()
{
    return _groupid;
}

void   WstConf::setgroupid(const string& id)
{
    _groupid = id;
}


// server type
string WstConf::servertype()
{
    return _type;
}

void   WstConf::setservertype(const string& type)
{
    _type = type;
}


// http path
string WstConf::httppath()
{
    return _httppath;
}

void   WstConf::sethttppath(const string& path)
{
    _httppath = path;
}


// http port
string WstConf::httpport()
{
    return _httpport;
}

void   WstConf::sethttpport(const string& port)
{
    _httpport = port;
}


// rtmp port
string WstConf::rtmpport()
{
    return _rtmpport;
}

void   WstConf::setrtmpport(const string& port)
{
    _rtmpport = port;
}

// free disk
uint32_t WstConf::freedisk()
{
    return _freedisk;
}

void     WstConf::setfreedisk(const int size)
{
    _freedisk = size;
}

uint32_t WstConf::keeplivetime()
{
    return _keeplive;
}

void     WstConf::setkeeplivetime(const int timeout)
{
    _keeplive = timeout;
}

uint32_t WstConf::idleLimitSec()
{
    return _idleLimitSec;
}

void     WstConf::setIdleLimitSec(const uint32_t sec)
{
    _idleLimitSec = sec;
}

// Blink app path
void WstConf::SetBlinkApp(std::string blinkapp) {
    _blinkexe = blinkapp;
}

std::string WstConf::GetBlinkApp() {
    return _blinkexe;
}

// Blink app id
void WstConf::SetBlinkAppId(std::string appid) {
    _blinkappid = appid;
}

std::string WstConf::GetBlinkAppId() {
    return _blinkappid;
}

// Blink channel id
void WstConf::SetBlinkChannelId(std::string channelid) {
    _blinkchannelid = channelid;
}

std::string WstConf::GetBlinkChannelId() {
    return _blinkchannelid;
}

// Blink token url
void WstConf::SetBlinkTokenUrl(std::string tokenurl) {
    _blinktokenurl = tokenurl;
}

std::string WstConf::GetBlinkTokenUrl() {
    return _blinktokenurl;
}

// Blink download url
void WstConf::SetBlinkDownloadUrl(std::string downloadurl) {
    _blinkdownloadurl = downloadurl;
}

std::string WstConf::GetBlinkDownloadUrl() {
    return _blinkdownloadurl;
}