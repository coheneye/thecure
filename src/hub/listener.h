#ifndef _THECURE_LISTENER_H_
#define _THECURE_LISTENER_H_

#include "hub.h"


class Listener {
public:
    explicit Listener(Hub* s);
    virtual ~Listener();

    int listen(const char* ip, unsigned short port);

protected:
    void* m_hot;   // handle of tcp of listening
    Hub* m_hub;
};

#endif