#ifndef _LOGIC_MANAGER_H_
#define _LOGIC_MANAGER_H_

#include <hub/manager.h>


class LogicManager: public Manager {
public:
    LogicManager(Hub* h);
    virtual ~LogicManager();

    virtual ISession* new_session();

    virtual void free_session(ISession* ses);
};

#endif