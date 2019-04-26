#include "manager.h"


Manager* Manager::m_inst = nullptr;


Manager::Manager()
{

}


Manager::~Manager()
{

}


Manager* Manager::inst()
{
    if(!m_inst){
        m_inst = new Manager();
    }
    return m_inst;
}


void Manager::add(Session* ses)
{
    
}


void Manager::remove(Session* ses)
{

}
