#ifndef _THECURE_SIGNAL_H_
#define _THECURE_SIGNAL_H_


#include <uv.h>


class Signal final{
public:
    Signal(uv_loop_t* l){
        uv_signal_init(l, &m_sig);
    }
    ~Signal(){
        uv_signal_stop(&m_sig);
    }

    void start(int signum){
        uv_signal_start(&m_sig, [](uv_signal_t* sig, int sn){
            
        }, signum);
    }
private:
    uv_signal_t m_sig;
};

#endif
