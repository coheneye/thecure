#ifndef _THECURE_SIGNAL_H_
#define _THECURE_SIGNAL_H_

#include <hub/hub.h>


class Signal {
public:
    Signal(Hub * h);
    ~Signal();

    int start(int sig, std::function<void(int)> callback);
    int oneshot(int sig, std::function<void(int)> callback);

    int stop();
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};


#endif
