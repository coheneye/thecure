#ifndef _THECURE_MANAGER_H_
#define _THECURE_MANAGER_H_

#include "session.h"

/**
 * @brief: session manager
 * 
 * MUST map user_id-->session satisfy O(1)
 * 
 */
class Manager {
private:
    explicit Manager();
    ~Manager();
public:
    static Manager* inst();

    void add(Session* ses);
    void remove(Session* ses);

protected:
    static Manager* m_inst;
};


#endif