#include "wstapp.h"
#include "wstrecord.h"
#include "wstconf.h"
#include "wstlog.h"

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

void WstApp::mainThread()
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
    
    while (!_isQuit)
    {
        statfs(WstConf::Instance().recordpath().c_str(), &diskinfo);
        unsigned long long totalBlocks = diskinfo.f_bsize;
        unsigned long long freeDisk = diskinfo.f_bfree * totalBlocks;
        size_t mbFreedisk = freeDisk>>20;
        if (mbFreedisk < WstConf::Instance().freedisk())
        {
            WstHttpClient::Instance().ReportStatus(DISKFULL, "disk full");
        }
        // cout << WstConf::Instance().recordpath() << "; free disk: " << mbFreedisk << endl;
        sleep(60*60);
    }
}

void WstApp::keepLiveThread()
{
    while (!_isQuit)
    {
        sleep(WstConf::Instance().keeplivetime());
        WstHttpClient::Instance().KeepLive();
    }
}

void WstApp::fSignalHandler(int signum)
{
    _isQuit = true;
   
    // LOG(logger::LOGDEBUG, "server app signal handler.");
    // std::cout << "server app signal handler." << std::endl;
}


void WstApp::initSignals()
{

}

int WstApp::parseOption(int argc, char ** argv)
{
    char *p;
    
    for (int i = 1; i < argc; i++)
    {
        p = argv[i];
        if (*p++ != '-')
        {
            return -1;
        }

        while (*p)
        {
            switch (*p++)
            {
            case '?':
            case 'h':
                cout << "show help" << endl;
                break;

            case 'v':
                cout << "show version" << endl;
                break;

            case 'q':
                cout << "show quit" << endl;
                break;

            case 's':
                cout << "show signal" << endl;
                return -1;

            default:
                cout << "show other" << endl;
                return -1;
            }
        }
    }

    return 0;
}

int WstApp::Run()
{
    WstLog::Instance().Initialize();
    WstConf::Instance().ReadConfigFile();
    WstConf::Instance().ReadLicenseFile();

    LOGW("server app start");

    // signal(SIGINT, fSignalHandler);
    std::thread worker(&WstApp::mainThread, this);
    std::thread sub1(&WstApp::checkDiskThread, this);
    std::thread sub2(&WstApp::keepLiveThread, this);
    worker.join();
    sub1.join();
    sub2.join();

    LOGW("server app stop");
    WstLog::Instance().Destroy();

    return 0;
}

int WstApp::Run(int argc, char ** argv)
{
    if (parseOption(argc, argv) != 0)
    {
        return 1;
    }

}