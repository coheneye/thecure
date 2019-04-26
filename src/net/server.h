#ifndef _THECURE_SERVER_H_
#define _THECURE_SERVER_H_

#include <uv.h>

/**
 * 
 */
class Server{
public:
    explicit Server();

    virtual ~Server();

    uv_loop_t * handle() const ;

    int serve_forever();
protected:
    uv_loop_t *m_hol;    // handle of loop
};

#endif