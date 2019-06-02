#ifndef __THECURE_IDLE_H_
#define __THECURE_IDLE_H_


#include "hub.h"


class Idle final {
public:
    Idle(Hub* h);
    ~Idle();

    int start(std::function<void(void*)> cb, void* param);
    int stop();
private:
    class IdleImpl;
    IdleImpl * m_impl;
};


#endif