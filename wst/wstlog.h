#ifndef WST_LOG_H
#define WST_LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "wstconf.h"

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

    std::string getTime();
    
    WstLog();

    ofstream    _outFile;
};

#define LOGW(msg)   \
    WstLog::Instance().Write(msg);


#endif // WST_LOG_H