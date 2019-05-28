#ifndef _THRCURE_TIMER_H_
#define _THRCURE_TIMER_H_

#include "hub.h"

class Timer final {
public:
    Timer(Hub* h);
    ~Timer();

public:
    int start(uint64_t timeout, uint64_t repeat, std::function<void(void*)> cb, void* data);
    int stop();
    int again();
    void set_interval(uint64_t repeat);
    uint64_t get_interval();
private:
    class TimerImpl;
    TimerImpl* m_impl;
};


#endif