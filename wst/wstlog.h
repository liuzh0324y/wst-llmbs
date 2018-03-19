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

    bool    Initialize(string filename="/opt/llmbs/logs/error.log");

    void    Destroy();

    int     Write(string message);

private:

    int     writeConsole(string message);

    int     writeFile(string message);

    WstLog();

    ofstream    _OutFile;
};

#define LOGW(msg)   \
    WstLog::Instance().Write(msg);

#endif // WST_LOG_H