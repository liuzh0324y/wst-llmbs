#ifndef WST_LOG_H
#define WST_LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class WstLog
{
public:
    static WstLog& Instance();

    bool    init(string filename="/opt/llmbs/logs/error.log");

    void    free();

    int     write(string message);

private:

    int     write_console(string message);

    int     write_file(string message);

    WstLog();

    ofstream    _OutFile;
};

#define LOGW(msg)   \
    WstLog::Instance().write(msg);

#endif // WST_LOG_H