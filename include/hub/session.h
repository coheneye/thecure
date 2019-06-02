#ifndef _THECURE_SESSION_H_
#define _THECURE_SESSION_H_

#include "hub.h"
#include "manager.h"
#include "dispatcher.h"


#define DEF_RECEIVE_BUF_SIZE 1600
#define DEF_SEND_BUF_SIZE DEF_RECEIVE_BUF_SIZE


/**
 * 
 * 
 */
class ISession {
public:
    ISession(Hub *h, IDispatcher* disp, Manager* m);
    virtual ~ISession();

    //
    int connect(const char* ip, int port);
    // stream mode
    int start_read();
    int send(const char* buf, unsigned int size);
    // close session. but not free memeory.
    
    int close();
    
    // session id.
    // unique within single process.
    int64_t id() const;

    void* handle();
    void* get_tag();
    void set_tag(void*);
protected:
    int inner_close();

public:
    void * m_hdl;
    void * m_writer;
    Hub* m_hub;
    Manager* m_mgr;
    IDispatcher* m_disp;
    void* m_tag;
};


#endif