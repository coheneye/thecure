#include "logic_server.h"
#include "logic_manager.h"


LogicServer::LogicServer():Server()
{

}

LogicServer::~LogicServer()
{

}

std::string LogicServer::get_lua_file()
{
    return "./lua/logic.lua";
}

Manager* LogicServer::create_manager()
{
    return new LogicManager(&m_hub);
}