#ifndef _THRCURE_TIMER_H_
#define _THRCURE_TIMER_H_

#include "hub.h"


class Timer {
public:
    Timer(Hub* h){
        uv_timer_init(h->handle(), &m_h);
        m_h.data = (void*)this;
    }
    ~Timer();

public:
    int start(uint64_t timeout);
    int stop();

    uint64_t get_interval(){

    }
protected:
    static void on_timer(uv_timer_t* h);

protected:
    uv_timer_t m_h;
};


#endif