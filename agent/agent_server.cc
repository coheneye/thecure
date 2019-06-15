#include "agent_server.h"
#include "serverside.h"


AgentServer::AgentServer():Server()
{

}


AgentServer::~AgentServer()=default;


std::string AgentServer::get_lua_file()
{
    return "./lua/main.lua";
}


Manager* AgentServer::create_manager()
{
    return new CManager(&m_hub);
}
