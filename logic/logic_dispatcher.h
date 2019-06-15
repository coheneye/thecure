#ifndef _LOGIC_DISPATCHER_H_
#define _LOGIC_DISPATCHER_H_

#include <hub/dispatcher.h>


class LogicDispathcher : public IDispatcher {
public:
    LogicDispathcher();
    virtual ~LogicDispathcher();

    virtual void dispatch(ISession* ses, char* data, int data_len);
};

#endif