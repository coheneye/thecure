#ifndef _AGENT_SERVERSIDE_H_
#define _AGENT_SERVERSIDE_H_

#include <hub/dispatcher.h>
#include <hub/session.h>


class ServerDispatcher : public IDispatcher{
public:
    virtual void dispatch(ISession* ses, char* data, int data_len);

private:   
};

class CManager : public Manager{
public:
    CManager(Hub* h);

    virtual ISession* new_session();

    virtual void free_session(ISession* ses);
    
};

#endif
