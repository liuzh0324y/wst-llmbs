#include "wstlog.h"
#include <ctime>

logger::logger()
{

}

logger& logger::instance()
{
    static logger theLogger;
    return theLogger;
}

bool    logger::init(string filename)
{
    _OutFile.open(filename, ofstream::out | ofstream::app);
    LOGW("load logger file.");
    return true;
}

void    logger::free()
{
    _OutFile.close();
}

int     logger::write(string message)
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

int     logger::write_console(string message)
{
    cout << message << endl;
    return message.length();
}

int     logger::write_file(string message)
{
    _OutFile << message << endl;
    _OutFile.flush();
    return message.length();
}