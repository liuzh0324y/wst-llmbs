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

bool    WstLog::Initialize(string filename)
{
    _outFile.open(filename, ofstream::out | ofstream::app);
    if (!_outFile)
    {
        LOGW("could not load logger file.");
    }
    else
    {
        LOGW("load logger file.");
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