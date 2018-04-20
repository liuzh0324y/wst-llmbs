#ifndef _WST_BLINK_RECORD_H_
#define _WST_BLINK_RECORD_H_

class WstBlinkRecord 
{
public:
    WstBlinkRecord();
    ~WstBlinkRecord();
   
    bool Initialize();

    bool Start();

    bool Stop();

    void Destroy();
};

#endif // _WST_BLINK_RECORD_H_