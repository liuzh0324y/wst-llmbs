#include "wstblinkrecord.h"
#include "wstdownloader.h"
#include <thread>

WstBlinkRecord::WstBlinkRecord() 
{
    _appid = "1234567890abcdefg";
    _uid = "downloaduser";
    _isQuit = false;
}

WstBlinkRecord::~WstBlinkRecord() 
{
    
}

bool WstBlinkRecord::Initialize()
{
    
}

bool WstBlinkRecord::Start()
{
    std::thread worker(&WstBlinkRecord::Worker, this);
    std::thread downloader(&WstBlinkRecord::Downloader, this);
    worker.join();
    downloader.join();
    return true;
}

bool WstBlinkRecord::Stop()
{

}

void WstBlinkRecord::Destroy()
{
    
}

void WstBlinkRecord::Worker() 
{
    // while (!_isQuit) {
    //     WstBlinkHandler handler;
    //     handler.run();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }
    
    WstHttpClient::Instance().GetBToken();
    while (!_isQuit) 
    {
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        WstHttpClient::Instance().GetDownloader();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // WstHttpClient::Instance().GetDownFile();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void WstBlinkRecord::Downloader() {

    while (!_isQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (WstHttpClient::Instance()._wstqueue.size() > 0) {
            WstValue value;
            value = WstHttpClient::Instance()._wstqueue.front();
            WstHttpClient::Instance()._wstqueue.pop();
            Handler(value);
        }
        
    }
}

bool ql = false;
void WstBlinkRecord::Handler(WstValue& value) {
    // if (!ql) {
    //     std::cout << "downloader url: " << value.url << std::endl;
    //     _downloader.run(value.cid, value.url, value.timestamp);
    //     ql = true;
    // }
    if (ql) return;
    int pos = value.url.find_last_of("/");
    std::string sname = value.url.substr(pos+1);
    std::cout << "source name: " << sname << std::endl;
    std::string name = "video";
    std::string command;
    command = "wget ";
    command.append(" -P ");
    command.append(name);
    command.append(" ");
    command.append(value.url);
    system(command.c_str());
    ql = true;
    std::cout << "system out" << std::endl;

    std::string convert;
    convert = "/opt/blink/videorecorder";
    convert.append(" video/");
    convert.append(sname);
    convert.append(" ");
    convert.append("test.flv");
    system(convert.c_str());
    std::cout << convert << "convert out" << std::endl; 
    // std::string url = "http://glxsslivelocal2.llvision.com:8800/token?appid=1234567890abcdefg&uid=downloader";
    // _downloader.run(value.cid, url, value.timestamp);
}