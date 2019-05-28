#ifndef _THECURE_DISPATCHER_H_
#define _THECURE_DISPATCHER_H_


class ISession;

/** Dispatcher 懂得逻辑，应该部分控制 Session 的动作 */
class IDispatcher {
public:
    IDispatcher();
    virtual ~IDispatcher();
    /** @buf: 为输出参数 */
    virtual void support(char* buf, unsigned int* len);

    virtual void dispatch(ISession* ses, char* data, int data_len) = 0;
};


class EchoDispatcher: public IDispatcher {
public:
    virtual void support(char* buf, int* len);
    virtual void dispatch(ISession* s, char* data, int data_len);
};


class LogicDispatcher: public IDispatcher {

};


#endif
