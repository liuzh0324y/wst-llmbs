#include "wstqueue.h"

WstQueue *WstQueue::Instance() {
    static WstQueue theQueue;
    return &theQueue;
}

void WstQueue::push(const WstValue &val) {
    _queue.push(val);
}

void WstQueue::pop() {
    _queue.pop();
}

WstValue& WstQueue::front() {
    return _queue.front();
}

int WstQueue::size() {
    return _queue.size();
}