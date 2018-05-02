#include "wstapp.h"
#include "wstrecord.h"
#include "wstconf.h"
#include "wstlog.h"
#include "wstblinkrecord.h"

#include <csignal>
#include <thread>
#include <mutex>
#include <sys/vfs.h>
#include <sys/stat.h>

bool WstApp::_isQuit = false;

WstApp::WstApp()
{

}

WstApp::~WstApp()
{

}

void WstApp::agoraServer()
{
    // http server
    WstHttpServer http;
    http.Initialize();
    http.Start();
    http.Stop();
    http.Destroy();
}

void WstApp::checkDiskThread()
{
    struct statfs diskinfo;
    // Determine if the folder exists.
    DIR *dir;
    if ((dir = opendir(WstConf::Instance().recordpath().c_str())) == NULL) {
        LOGW(WstConf::Instance().recordpath() + " not exist");
        return;
    }
    closedir(dir);
    
    //
    sleep(60*10);
    while (!_isQuit)
    {
        statfs(WstConf::Instance().recordpath().c_str(), &diskinfo);
        unsigned long long totalBlocks = diskinfo.f_bsize;
        unsigned long long freeDisk = diskinfo.f_bfree * totalBlocks;
        size_t mbFreedisk = freeDisk>>20;
        if (mbFreedisk < WstConf::Instance().freedisk())
        {
            WstHttpClient::Instance().ReportStatus(DISKFULL, NULL, NULL, "disk full");
        }
        // cout << WstConf::Instance().recordpath() << "; free disk: " << mbFreedisk << endl;
        sleep(60*60);
    }
}

void WstApp::keepLiveThread()
{
    sleep(60*10);
    while (!_isQuit)
    {
        sleep(WstConf::Instance().keeplivetime());
        WstHttpClient::Instance().KeepLive();
    }
}

void WstApp::blinkServer()
{
    WstBlinkRecord blink;
    if (blink.Initialize()) {
        blink.Start();
    }
    _isQuit = true;
    blink.Stop();
    blink.Destroy();
}

int WstApp::Run()
{
    WstLog::Instance().Initialize();
    WstConf::Instance().ReadConfigFile();
    WstConf::Instance().ReadLicenseFile();

    std::thread checkdist(&WstApp::checkDiskThread, this);
    std::thread keeplive(&WstApp::keepLiveThread, this);

    if (WstConf::Instance().servertype().compare("ambs") == 0) {
        std::thread server(&WstApp::agoraServer, this);
        LOGW("agora server app start");
        server.join();
    } else if (WstConf::Instance().servertype().compare("bmbs") == 0) {
        std::thread server(&WstApp::blinkServer, this);
        LOGW("blink server app start");
        server.join();
    }

    checkdist.join();
    keeplive.join();

    LOGW("server app stop");
    _isQuit = true;
    WstLog::Instance().Destroy();

    return 0;
}