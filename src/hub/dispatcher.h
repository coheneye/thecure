#ifndef _THECURE_DISPATCHER_H_
#define _THECURE_DISPATCHER_H_


class ISession;

/** Dispatcher 懂得逻辑，应该部分控制 Session 的动作 */
class IDispatcher {
public:
    IDispatcher();
    virtual ~IDispatcher();
    /** @buf: 为输出参数 */
    virtual void lend(char* buf, unsigned int* len);
    virtual void remand(char* buf, unsigned int len);

    virtual void dispatch(ISession* ses, char* data, int data_len);
};


#endif
