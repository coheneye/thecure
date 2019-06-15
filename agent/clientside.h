#ifndef _AGENT_CLIENTSIDE_H
#define _AGENT_CLIENTSIDE_H

#include <hub/dispatcher.h>
#include <hub/session.h>


/**
 * 客户端消息 dispatcher 
 * 
 */
class ClientDispatcher : public IDispatcher{
public:
    ClientDispatcher();
    virtual ~ClientDispatcher();

    virtual void dispatch(ISession* ses, char* data, int data_len);
};


class SManager : public Manager{
public:
    SManager(Hub* b);
    virtual ISession* new_session();
    virtual void free_session(ISession* ses);
};


#endif
