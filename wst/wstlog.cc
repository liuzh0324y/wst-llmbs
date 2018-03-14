#include "wstlog.h"
#include <ctime>

WstLog::WstLog()
{

}

WstLog& WstLog::Instance()
{
    static WstLog theLogger;
    return theLogger;
}

bool    WstLog::init(string filename)
{
    _OutFile.open(filename, ofstream::out | ofstream::app);
    LOGW("load logger file.");
    return true;
}

void    WstLog::free()
{
    _OutFile.close();
}

int     WstLog::write(string message)
{
    ostringstream oss;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    oss << "[" << 1900+ltm->tm_year << ":" << 1+ltm->tm_mon << ":" << ltm->tm_mday << 
            "-" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] " << message;
    write_console(oss.str());
    write_file(oss.str());
    return 0;
}

int     WstLog::write_console(string message)
{
    cout << message << endl;
    return message.length();
}

int     WstLog::write_file(string message)
{
    _OutFile << message << endl;
    _OutFile.flush();
    return message.length();
}