#ifndef _THECURE_MANAGER_H_
#define _THECURE_MANAGER_H_


#include "hub.h"
#include <unordered_map>


class ISession;
class IDispatcher;
/**
 * @brief: session manager
 * 
 * MUST map user_id-->session satisfy O(1)
 * 
 */

class Manager {
public:
    explicit Manager(Hub* b);
    ~Manager();
public:
    // used for server side session manager
    int do_accept(void* s);
    // used for client session manager
    int do_connected(ISession* ses);
    int do_connection_lost(ISession* ses);

    virtual ISession* new_session();
    virtual void free_session(ISession* ses);

    ISession* get_session(int64_t id);

    int do_read_error(ISession* ses, int status);
    int do_write_error(ISession* ses, int status);
    int do_close(ISession* ses);
    
    /**> verify user success */
    int do_update_session_tag(ISession* ses, void* tag);
    ISession* get_session_by_tag(void* tag);
    /**> kick */
    int remove_session(ISession* ses);

protected:
    Hub* m_hub;
    std::unordered_map<int64_t, ISession*> m_sessions;
    std::unordered_map<void*, ISession*> m_tag_sessions;
};


#endif