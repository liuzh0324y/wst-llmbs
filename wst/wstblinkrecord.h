#ifndef _WST_BLINK_RECORD_H_
#define _WST_BLINK_RECORD_H_

#include "wstagoraclient.h"
#include "wstdownloader.h"

#include <string>

class WstBlinkRecord 
{
public:
    WstBlinkRecord();
    ~WstBlinkRecord();
   
    bool Initialize();

    bool Start();

    bool Stop();

    void Destroy();

protected:
    void Worker();
    void Downloader();

private:
    void Handler(WstValue& value);
    
private:
    std::string     _appid;
    std::string     _uid;
    bool            _isQuit;
    WstDownloader   _downloader;
};

#endif // _WST_BLINK_RECORD_H_