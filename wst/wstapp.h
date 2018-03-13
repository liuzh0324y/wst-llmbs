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
    int ParseOption(int argc, char ** argv);
    void InitSignals();
    int Daemon();

protected:
    void main_thread();
    void sub1_thread();
    void sub2_thread();
    static void fSignalHandler(int signum);


private:
    // quit flag
    static bool _is_quit;
};

#endif // WST_APP_H