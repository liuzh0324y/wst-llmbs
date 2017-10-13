#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_

#include "httpserver.h"

class AgoraRecorder;

class application
{
public:
    application();
    ~application();

    int run();

protected:
    void main_thread();
    void sub1_thread();
    void sub2_thread();
    static void signalHandler(int signum);
    
private:
    // quit flag
    static bool _is_quit;
};

#endif // _SERVERAPP_H_