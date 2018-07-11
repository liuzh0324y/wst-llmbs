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
    std::string logName;
    currentDate = GetDate();
    logName = filepath;
    logName.append("/");
    logName.append(currentDate);
    logName.append("_error.log");

    _outFile.open(logName, ofstream::out | ofstream::app);
    if (!_outFile)
    {
        std::stringstream tmp;
        tmp << "Could not open " << logName;
        LOGW(tmp.str());
    }
    else
    {
        std::stringstream tmp;
        tmp << "Open the log file " << logName;
        LOGW(tmp.str());
    }
    
    return true;
}

void    WstLog::Destroy()
{
    _outFile.close();
}

int     WstLog::Write(string message)
{
    if (currentDate.compare(GetDate()) != 0)
    {
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
    if (_outFile.is_open())
    {
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

std::string WstLog::GetDate()
{
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t(system_clock::now());

    struct std::tm *ptm = std::localtime(&tt);
    std::stringstream ss;
    ss << std::put_time(ptm, "%Y-%m-%d");
    return ss.str();
}