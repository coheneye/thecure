#ifndef _THRCURE_TIMER_H_
#define _THRCURE_TIMER_H_

#include "hub.h"
#include <stdint.h>

class Timer {
public:
    Timer(Hub* h);
    ~Timer();

public:
    int start(uint64_t timeout);
    int stop();

    uint64_t get_interval();
protected:
protected:
    void* m_timer;
};


#endif