#ifndef _THECURE_HUB_H_
#define _THECURE_HUB_H_

#include <stdint.h>
#include <functional>


/**
 * 
 */
class Hub {
public:
    explicit Hub();

    virtual ~Hub();

    void * handle() const ;

    int serve_forever();
    int stop();
    
protected:
    void *m_hol;    // handle of loop
};


#endif