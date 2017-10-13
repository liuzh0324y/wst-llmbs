#include "loadconf.h"

loadconf *loadconf::_instance = NULL;
mutex loadconf::_lock;

loadconf::loadconf()
{
    _seqnum = 0;
    _freedisk = 300;
    _keeplive = 3600;
}

loadconf::~loadconf()
{

}

loadconf& loadconf::instance()
{    
    static loadconf theloadconf;
    return theloadconf;
}

bool loadconf::load_config_file(string file)
{
    vector<string> filevec;
    _input.open(file);

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

    _parse_conf(filevec);
    LOGW("load config file.");
    return true;
}

bool loadconf::load_license_file(string file)
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
    
    LOGW("load license file.");
    return true;
}

string loadconf::_parse_conf(string input)
{
    if (input.empty())
    {
        return string();
    }
    if ((input.compare(0, 1, "#") == 0))
    {
        return string();
    }
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
        else if (temp.compare("logs_path") == 0)
        {
            _logspath = tempstr;
        }
        else if (temp.compare("applite_path") == 0)
        {
            _applitepath = tempstr;
        }
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
    }
    return string();
}

string loadconf::_parse_conf(vector<string> &filelist)
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
            else if (temp.compare("logs_path") == 0)
            {
                _logspath = tempstr;
            }
            else if (temp.compare("applite_path") == 0)
            {
                _applitepath = tempstr;
            }
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
            else if (temp.compare("freedisk") == 0)
            {
                _freedisk = string_as_T<uint32_t>(tempstr);
            }
            else if (temp.compare("keeplive") == 0)
            {
                _keeplive = string_as_T<uint32_t>(tempstr);
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

string loadconf::number()
{
    char cpuid[100];
    getcpuid(cpuid);

    return cpuid;
}

uint32_t loadconf::seqnum()
{
    return _seqnum++;
}

// user name
string loadconf::username()
{
    return _username;
}

void   loadconf::setusername(const string& name)
{
    _username = name;
}


// user password
string loadconf::password()
{
    return _password;
}

void   loadconf::setpassword(const string& password)
{
    _password = password;
}


// local ip
string loadconf::localip()
{
    return _localip;
}

void   loadconf::setlocalip(const string& ip)
{
    _localip = ip;
}


// local port
string loadconf::localport()
{
    return _localport;
}

void   loadconf::setlocalport(const string& port)
{
    _localport = port;
}


// server ip
string loadconf::serverip()
{
    return _serverIp;
}

void   loadconf::setserverip(const string& ip)
{
    _serverIp = ip;
}


// server port
string loadconf::serverport()
{
    return _serverPort;
}

void   loadconf::setserverport(const string& port)
{
    _serverPort = port;
}


// connect timeout
uint32_t loadconf::connecttimeout()
{
    return _connectTimeout;
}

void   loadconf::setconnectimeout(const uint32_t& timeout)
{
    _connectTimeout = timeout;
}


// send timeout
uint32_t loadconf::sendtimeout()
{
    return _sendTimeout;
}

void   loadconf::setsendtimeout(const uint32_t& timeout)
{
    _sendTimeout = timeout;
}


// recv timeout
uint32_t loadconf::recvtimeout()
{
    return _recvTimeout;
}

void   loadconf::setrecvtimeout(const uint32_t& timeout)
{
    _recvTimeout = timeout;
}


// record path
string loadconf::recordpath()
{
    return _recordpath;
}

void   loadconf::setrecordpath(const string& path)
{
    _recordpath = path;
}


// logs path
string loadconf::logspath()
{
    return _logspath;
}

void   loadconf::setlogspath(const string& path)
{
    _logspath = path;
}


// applite path
string loadconf::applitepath()
{
    return _applitepath;
}

void   loadconf::setapplitepath(const string& path)
{
    _applitepath = path;
}


// group id
string loadconf::groupid()
{
    return _groupid;
}

void   loadconf::setgroupid(const string& id)
{
    _groupid = id;
}


// server type
string loadconf::servertype()
{
    return _type;
}

void   loadconf::setservertype(const string& type)
{
    _type = type;
}


// http path
string loadconf::httppath()
{
    return _httppath;
}

void   loadconf::sethttppath(const string& path)
{
    _httppath = path;
}


// http port
string loadconf::httpport()
{
    return _httpport;
}

void   loadconf::sethttpport(const string& port)
{
    _httpport = port;
}


// rtmp port
string loadconf::rtmpport()
{
    return _rtmpport;
}

void   loadconf::setrtmpport(const string& port)
{
    _rtmpport = port;
}

// free disk
uint32_t loadconf::freedisk()
{
    return _freedisk;
}

void     loadconf::setfreedisk(const int size)
{
    _freedisk = size;
}

uint32_t loadconf::keeplivetime()
{
    return _keeplive;
}

void     loadconf::setkeeplivetime(const int timeout)
{
    _keeplive = timeout;
}