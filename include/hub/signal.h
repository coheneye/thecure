#ifndef _THECURE_SIGNAL_H_
#define _THECURE_SIGNAL_H_

#include <hub/hub.h>


class Signal {
public:
    Signal(Hub * h);
    ~Signal();

    int start(int sig, function<void(int)> callback);
    int oneshot(int sig, function<void(int)> callback);

    int stop();
private:
    class Impl;
    unique_ptr<Impl> m_impl;
};


#endif
