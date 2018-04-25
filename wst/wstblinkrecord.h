#ifndef _WST_BLINK_RECORD_H_
#define _WST_BLINK_RECORD_H_

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

private:
    std::string     _appid;
    std::string     _uid;
    bool            _isQuit;
};

#endif // _WST_BLINK_RECORD_H_