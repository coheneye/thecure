#ifndef _THECURE_DISPATCHER_H_
#define _THECURE_DISPATCHER_H_

#include <uv.h>
class ISession;

/** Dispatcher 懂得逻辑，应该部分控制 Session 的动作 */
class IDispatcher {
public:
    IDispatcher();
    virtual ~IDispatcher();
    /** @buf: 为输出参数 */
    virtual void support(uv_buf_t* buf);

    virtual void dispatch(ISession* ses, char* data, size_t data_len) = 0;
};


class EchoDispatcher: public IDispatcher {
public:
    virtual void support(uv_buf_t* buf);
    virtual void dispatch(uv_stream_t* s, char* data, size_t data_len);
};


class LogicDispatcher: public IDispatcher {

};


#endif
