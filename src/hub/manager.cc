#include "manager.h"
#include "session.h"
#include <uv.h>
#include <iostream>


Manager::Manager(Hub* h):m_hub(h)
{
    
}


Manager::~Manager()
{
    for(auto pair:m_sessions){
        this->free_session(pair.second);
    }
    m_sessions.clear();
    m_tag_sessions.clear();
}

int Manager::do_accept(void* s)
{
    std::cout<<"do_accept" <<std::endl;
    ISession* ses = this->new_session();
    if(!ses){
        //@TODO LOG
        std::cout<< "do_accept() error" << std::endl;
        return -1;
    }
    int ret = uv_accept((uv_stream_t*)s, (uv_stream_t*)ses->handle());
    if(ret){
        std::cout << "uv_accept failed" << uv_err_name(ret) << std::endl;
        return ret;
    }

    m_sessions[ses->id()] = ses;

    ret = ses->start_read();
    return ret;
}


int Manager::do_connected()
{

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
    m_tag_sessions.erase(ses->get_tag());
}


/**> verify user success */
int Manager::do_update_session_tag(ISession* ses, void* tag)
{
    ses->set_tag(tag);
    m_tag_sessions[tag] = ses;  // insert
}

ISession* Manager::get_session_by_tag(void* tag)
{
    auto it = m_tag_sessions.find(tag);
    if(it == m_tag_sessions.end()){
        return nullptr;
    }
    return it->second;
}


/**> kick */
int Manager::remove_session(ISession* ses)
{
    ses->close();
}