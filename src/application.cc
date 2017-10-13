#include "application.h"
#include "recorder.h"
#include "loadconf.h"
#include "logger.h"

#include <csignal>
#include <thread>
#include <mutex>
#include <sys/vfs.h>
#include <sys/stat.h>

bool application::_is_quit = false;

application::application()
{

}

application::~application()
{

}

void application::main_thread()
{
    // http server
    httpserver http;
    http.http_server_init();
    http.http_server_run();
    http.http_server_stop();
    http.http_server_free();
}

void application::sub1_thread()
{
    struct statfs diskinfo;
    // DIR *dir = opendir(loadconf::instance().recordpath().c_str());
    // if (dir == NULL)
    // {
    //     int beginCmpPath;
    //     int endCmpPath;
    //     int fullPathLen;
    //     int pathLen = loadconf::instance().recordpath().length();
    //     char currentPath[128] = {0};
    //     char fullPath[128] = {0};
    //     if (loadconf::instance().recordpath().at(0) != '/')
    //     {
    //         getcwd(currentPath, sizeof(currentPath));
    //         strcat(currentPath, "/");
    //         beginCmpPath = strlen(currentPath);
    //         strcat(currentPath, loadconf::instance().recordpath().c_str());
    //         if (loadconf::instance().recordpath().at(pathLen) != '/')
    //         {
    //             strcat(currentPath, "/");
    //         }
    //         endCmpPath = strlen(currentPath);
    //     }
    //     else
    //     {
    //         strcpy(currentPath, loadconf::instance().recordpath().c_str());
    //         if (loadconf::instance().recordpath().at(loadconf::instance().recordpath().length()) != '/')
    //         {
    //             strcat(currentPath, "/");
    //         }
    //         beginCmpPath = 1;
    //         endCmpPath = strlen(currentPath);
    //     }
    //     for (int i = beginCmpPath; i < endCmpPath; i++)
    //     {
    //         if (currentPath[i] == '/')
    //         {
    //             currentPath[i] = '\0';
    //             if (access(currentPath, NULL) != 0)
    //             {
    //                 if (mkdir(currentPath, 0755) == -1)
    //                 {

    //                 }
    //             }
    //             currentPath[i] = '/';
    //         }
    //     }
    // }
    // closedir(dir);
    while (!_is_quit)
    {
        statfs(loadconf::instance().recordpath().c_str(), &diskinfo);
        unsigned long long totalBlocks = diskinfo.f_bsize;
        unsigned long long freeDisk = diskinfo.f_bfree * totalBlocks;
        size_t mbFreedisk = freeDisk>>20;
        if (mbFreedisk < loadconf::instance().freedisk())
        {
            httpclient::instance().reportstaus(DISKFULL);
        }
        // cout << loadconf::instance().recordpath() << "; free disk: " << mbFreedisk << endl;
        sleep(60*60);
    }
}

void application::sub2_thread()
{
    while (!_is_quit)
    {
        sleep(loadconf::instance().keeplivetime());
        httpclient::instance().keeplive();
    }
}

void application::signalHandler(int signum)
{
    _is_quit = true;
   
    // LOG(logger::LOGDEBUG, "server app signal handler.");
    // std::cout << "server app signal handler." << std::endl;
}

int application::run()
{
    logger::instance().init();
    loadconf::instance().load_config_file();
    loadconf::instance().load_license_file();

    LOGW("server app start");

    // signal(SIGINT, signalHandler);
    std::thread worker(&application::main_thread, this);
    std::thread sub1(&application::sub1_thread, this);
    std::thread sub2(&application::sub2_thread, this);
    worker.join();
    sub1.join();
    sub2.join();

    LOGW("server app stop");
    logger::instance().free();

    return 0;
}