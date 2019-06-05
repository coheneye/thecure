#ifndef _THECURE_HUB_H_
#define _THECURE_HUB_H_

#include <stdint.h>
#include <functional>
#include <memory>

using namespace std;

// golang-like defer
using defer = std::shared_ptr<void>;

// make_unique
template<typename T, typename ...Args>
unique_ptr<T> make_unique(Args&& ...args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

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