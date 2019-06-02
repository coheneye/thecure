#ifndef _THECURE_HUB_H_
#define _THECURE_HUB_H_

#include <stdint.h>
#include <functional>
#include <memory>

using defer = std::shared_ptr<void>;

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
    void *m_hdl;    // handle of loop
};


#endif