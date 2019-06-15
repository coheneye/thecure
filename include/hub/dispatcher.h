#ifndef _THECURE_DISPATCHER_H_
#define _THECURE_DISPATCHER_H_

#include <cstdint>


class ISession;

/** Dispatcher 懂得逻辑，应该部分控制 Session 的动作 */
class IDispatcher {
public:
    IDispatcher();
    virtual ~IDispatcher();
    /** @buf: 为输出参数 */
    virtual void lend(char** buf, unsigned int* len);
    virtual void remand(char* buf, unsigned int len);

    virtual void on_data_read(ISession* ses, char* data, int data_len);
    virtual void dispatch(ISession* ses, char* data, int data_len);

protected:
    uint32_t m_need;
    uint32_t m_received;
    
    char* m_buf;

    bool m_header;
};


#endif
