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
private:
    explicit Manager(Hub* b);
    ~Manager();
public:
    int do_accept(void* s);
    int do_connected();

    virtual ISession* new_session();
    virtual void free_session(ISession* ses);

    ISession* get_session(int64_t id);

    int do_read_error(ISession* ses, int status);
    int do_write_error(ISession* ses, int status);
    int do_close(ISession* ses);
    
    /**> verify user success */
    int do_update_user(ISession* ses, uint64_t user_id);
    ISession* get_user_session(uint64_t user_id);
    /**> kick */
    int remove_session(ISession* ses);

protected:
    Hub* m_hub;
    std::unordered_map<int64_t, ISession*> m_sessions;
    std::unordered_map<uint64_t, ISession*> m_user_session;
};


#endif