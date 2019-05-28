#include "manager.h"


Manager::Manager(Hub* h):m_hub(h)
{
    
}


Manager::~Manager()
{

}


ISession* Manager::new_session()
{

}


void Manager::free_session(ISession* ses)
{

}

ISession* Manager::get_session(int64_t id)
{

}

int Manager::do_read_error(ISession* ses, int status)
{

}

int Manager::do_write_error(ISession* ses, int status)
{

}

int Manager::do_close(ISession* ses)
{

}

/**> verify user success */
int Manager::do_update_user(ISession* ses)
{

}

/**> kick */
int Manager::remove_session(ISession* ses)
{

}