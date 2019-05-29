#include "manager.h"
#include "session.h"


Manager::Manager(Hub* h):m_hub(h)
{
    
}


Manager::~Manager()
{
    for(auto pair:m_sessions){
        this->free_session(pair.second);
    }
    m_sessions.clear();
}


ISession* Manager::new_session()
{
    return new ISession(m_hub, new IDispatcher(), this);
}


void Manager::free_session(ISession* ses)
{
    delete ses;
    ses = 0;
}


ISession* Manager::get_session(int64_t id)
{
    auto it = m_sessions.find(id);
    if(it == m_sessions.end()){
        return nullptr;
    }
    return it->second;
}


int Manager::do_read_error(ISession* ses, int status)
{
    ses->close();
}


int Manager::do_write_error(ISession* ses, int status)
{
    ses->close();
}


int Manager::do_close(ISession* ses)
{
    this->free_session(ses);

    m_sessions.erase(ses->id());
}


/**> verify user success */
int Manager::do_update_user(ISession* ses, uint64_t user_id)
{
    ses->set_tag((void*)user_id);
}

ISession* Manager::get_user_session(uint64_t user_id)
{
    auto it = m_user_session.find(user_id);
    if(it == m_user_session.end()){
        return nullptr;
    }
    return it->second;
}


/**> kick */
int Manager::remove_session(ISession* ses)
{
    ses->close();
}