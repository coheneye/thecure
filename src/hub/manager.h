#ifndef _THECURE_MANAGER_H_
#define _THECURE_MANAGER_H_


#include <unordered_map>

class ISession;

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
    int do_accept(void* s);
    int do_connected();

    virtual ISession* new_session();
    virtual void free_session(ISession* ses);

    ISession* get_session(int64_t id);

    int do_read_error(ISession* ses, int status);
    int do_write_error(ISession* ses, int status);
    int do_close(ISession* ses);
    
    /**> verify user success */
    int do_update_user(ISession* ses);
    /**> kick */
    int remove_session(ISession* ses);

protected:
    std::unordered_map<int64_t, ISession*> m_sessions;
};


#endif