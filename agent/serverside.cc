#include "serverside.h"
#include <iostream>


ServerDispatcher::ServerDispatcher():IDispatcher()
{

}


ServerDispatcher::~ServerDispatcher() = default;


void ServerDispatcher::dispatch(ISession* ses, char* data, int data_len){
    std::cout << data << data_len << std::endl;
}


CManager::CManager(Hub* h):Manager(h)
{
}

ISession* CManager::new_session()
{
    std::cout<<"new new_session" << std::endl;
    return new ISession(this->m_hub, new ServerDispatcher(), this);
}

void CManager::free_session(ISession* ses)
{
    delete ses->m_disp;
    delete ses;
    ses = 0;
}
