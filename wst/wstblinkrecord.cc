#include "wstblinkrecord.h"
#include "wstagoraclient.h"
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
    worker.join();
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
    WstBlinkHandler handler;
    handler.run();
    // while (!_isQuit) 
    // {
    //     WstHttpClient::Instance().GetBToken();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    //     WstHttpClient::Instance().GetDownloader();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    //     // WstHttpClient::Instance().GetDownFile();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }
}