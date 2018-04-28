#ifndef WST_QUEUE_H
#define WST_QUEUE_H

#include "config.h"

class WstValue {
public:
    std::string cid;
    std::string url;
    std::string timestamp;
};

class WstQueue {
public:
    static WstQueue *Instance();

    void push(const WstValue &val);

    void pop();

    WstValue& front();

    int size();
private:
    std::queue<WstValue> _queue;
};

#endif // WST_QUEUE_H