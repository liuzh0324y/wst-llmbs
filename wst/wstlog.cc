#include "wstlog.h"
#include <ctime>
#include <sys/stat.h>
#include <iomanip>
#include <chrono>

WstLog::WstLog()
{

}

WstLog& WstLog::Instance()
{
    static WstLog theLogger;
    return theLogger;
}

bool    WstLog::Initialize(string filepath)
{
    std::string logname;
    _currentDate = GetDate();
    logname = filepath;
    logname.append("/");
    logname.append(_currentDate);
    logname.append("_error.log");
    _outFile.open(logname, ofstream::out | ofstream::app);
    if (!_outFile)
    {
        std::stringstream tmp;
        tmp << "could not open " << logname;
        LOGW(tmp.str());
    }
    else
    {
        std::stringstream tmp;
        tmp << "open the log file " << logname;
        LOGW(tmp.str());
    }
    
    return true;
}

void    WstLog::Destroy()
{
    if (_outFile.is_open()) {
        _outFile.close();
    }
    
}

int     WstLog::Write(string message)
{
    if (_currentDate.compare(GetDate()) != 0) {
        Destroy();
        Initialize();
    }
    std::stringstream msg;
    msg << getTime() << " --> " << message;
    writeConsole(msg.str());
    writeFile(msg.str());
    return 0;
}

int     WstLog::writeConsole(string message)
{
    cout << message << endl;
    return message.length();
}

int     WstLog::writeFile(string message)
{
    if (_outFile.is_open()) {
        _outFile << message << endl;
        _outFile.flush();
    }
    
    return message.length();
}

std::string WstLog::getTime()
{
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t(system_clock::now());

    struct std::tm *ptm = std::localtime(&tt);
    std::stringstream ss;
    ss << std::put_time(ptm, "[%Y-%m-%d %H.%M.%S]");
    return ss.str();
}

std::string WstLog::GetDate() {
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t(system_clock::now());

    struct std::tm *ptm = std::localtime(&tt);
    std::stringstream ss;
    ss << std::put_time(ptm, "%Y-%m-%d");
    return ss.str();
}