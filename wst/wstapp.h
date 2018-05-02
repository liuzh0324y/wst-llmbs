#ifndef WST_APP_H
#define WST_APP_H

#include "wstagoraserver.h"

class AgoraRecorder;

class WstApp
{
public:
    WstApp();
    ~WstApp();

    int Run();

protected:
    void agoraServer();
    void blinkServer();
    void checkDiskThread();
    void keepLiveThread();

private:
    // quit flag
    static bool _isQuit;
};

#endif // WST_APP_H