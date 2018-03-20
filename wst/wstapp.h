#ifndef WST_APP_H
#define WST_APP_H

#include "wsthttp.h"

class AgoraRecorder;

class WstApp
{
public:
    WstApp();
    ~WstApp();

    int Run();
    int Run(int argc, char ** argv);

private:
    int     parseOption(int argc, char ** argv);
    void    initSignals();

protected:
    void mainThread();
    void sub1Thread();
    void sub2Thread();
    static void fSignalHandler(int signum);


private:
    // quit flag
    static bool _isQuit;
};

#endif // WST_APP_H