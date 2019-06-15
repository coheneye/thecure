#include "logic_manager.h"
#include <hub/session.h>
#include "logic_dispatcher.h"


LogicManager::LogicManager(Hub* h):Manager(h)
{

}


LogicManager::~LogicManager()=default;


ISession* LogicManager::new_session()
{
    return new ISession(this->m_hub, new LogicDispathcher(), this);
}


void LogicManager::free_session(ISession* ses)
{
    delete ses->m_disp;
    delete ses;
    ses = 0;
}