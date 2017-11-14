#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class logger
{
public:
    static logger& instance();

    bool    init(string filename="/usr/local/llmbs/logs/error.log");

    void    free();

    int     write(string message);

private:

    int     write_console(string message);

    int     write_file(string message);

    logger();

    ofstream    _OutFile;
};

#define LOGW(msg)   \
    logger::instance().write(msg);

#endif // _LOGGER_H_