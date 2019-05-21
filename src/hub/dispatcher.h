#ifndef _THECURE_DISPATCHER_H_
#define _THECURE_DISPATCHER_H_

#include <uv.h>


class IDispatcher {
public:
    IDispatcher();
    virtual ~IDispatcher();

    virtual void dispatch(uv_stream_t* s, char* data, size_t data_len) = 0;
};


class EchoDispatcher: public IDispatcher {
public:
    virtual void dispatch(uv_stream_t* s, char* data, size_t data_len);
};


class LogicDispatcher: public IDispatcher {

};


#endif
