#ifndef _THECURE_HUB_H_
#define _THECURE_HUB_H_

#include <uv.h>
#include <singleton.h>

/**
 * 
 */
class Hub {
public:
    explicit Hub();

    virtual ~Hub();

    uv_loop_t * handle() const ;

    virtual int serve_forever();
protected:
    uv_loop_t *m_hol;    // handle of loop
};


#endif