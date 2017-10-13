#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_

#include "httpserver.h"

class AgoraRecorder;

class Application
{
public:
    Application();
    ~Application();

    int Run();
    int Run(int argc, char ** argv);

protected:
    void main_thread();
    void sub1_thread();
    void sub2_thread();
    static void signalHandler(int signum);
    int ParseOption(int argc, char ** argv);
private:
    // quit flag
    static bool _is_quit;
};

#endif // _SERVERAPP_H_