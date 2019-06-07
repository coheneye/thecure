#ifndef _AGENT_CLIENTSIDE_H
#define _AGENT_CLIENTSIDE_H

#include <hub/dispatcher.h>
#include <hub/session.h>


class ClientDispatcher : public IDispatcher{
public:
    virtual void dispatch(ISession* ses, char* data, int data_len);
private:
};


class SManager : public Manager{
public:
    SManager(Hub* b);
    virtual ISession* new_session();
    virtual void free_session(ISession* ses);
};


#endif
