#include "clientside.h"
#include <iostream>
#include <utils/function.h>


void ClientDispatcher::dispatch(ISession* ses, char* data, int data_len)
{
    std::cout << data << data_len << std::endl;
}


SManager::SManager(Hub* b):Manager(b)
{
}

ISession* SManager::new_session()
{
    std::cout<<"new new_session" << std::endl;
    return new ISession(this->m_hub, new ClientDispatcher(), this);
}

void SManager::free_session(ISession* ses)
{
    delete ses->m_disp;
    delete ses;
    ses = 0;
}
