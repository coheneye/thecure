#ifndef _THECURE_LISTENER_H_
#define _THECURE_LISTENER_H_

#include "server.h"


class Listener{
public:
    explicit Listener(Server* s);
    virtual ~Listener();

    int listen(const char* ip, unsigned short port);

protected:
    uv_tcp_t m_hot;   // handle of tcp
    Server* m_server;
};

#endif