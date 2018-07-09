#include "wstconf.h"
#include "wstlog.h"

WstConf *WstConf::_instance = NULL;
mutex WstConf::_lock;

WstConf::WstConf()
{
    _seqnum = 0;
    _freedisk = 300;
    _keeplive = 3600;
    _idleLimitSec = 600;
    
    _username = "admin";
    _password = "123456";
    _localip = "127.0.0.1";
    _localport = "18005";
    _serverIp = "127.0.0.1";
    _serverPort = "18003";
    _goserverip = "127.0.0.1";
    _goserverport = "8888";
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
        temp << "could not load file. " << file;
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
            else if (temp.compare("local_ip") == 0)
            {
                _localip = tempstr;
            }
            else if (temp.compare("local_port") == 0)
            {
                _localport = tempstr;
            }
            else if (temp.compare("server_ip") == 0)
            {
                _serverIp = tempstr;
            }
            else if (temp.compare("server_port") == 0)
            {
                _serverPort = tempstr;
            }
            else if (temp.compare("goserverip") == 0)
            {
                _goserverip = tempstr;
            }
            else if (temp.compare("goserverport") == 0)
            {
                _goserverport = tempstr;
            }
            else if (temp.compare("connect_timeout") == 0)
            {
                _connectTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("send_timeout") == 0)
            {
                _sendTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("recv_timeout") == 0)
            {
                _recvTimeout = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("record_path") == 0)
            {
                _recordpath = tempstr;
            }
            // else if (temp.compare("logs_path") == 0)
            // {
            //     _logspath = tempstr;
            // }
            // else if (temp.compare("applite_path") == 0)
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
            else if (temp.compare("http_path") == 0)
            {
                _httppath = tempstr;
            }
            else if (temp.compare("http_port") == 0)
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

// go server ip
string WstConf::goserverip()
{
    return _goserverip;
}

void   WstConf::setgoserverip(const string& ip)
{
    _goserverip = ip;
}

// go server port
string WstConf::goserverport()
{
    return _goserverport;
}

void   WstConf::setgoserverport(const string& port)
{
    _goserverport = port;
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