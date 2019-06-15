#ifndef _AGENT_SERVER_H_
#define _AGENT_SERVER_H_

#include <server.h>


class AgentServer : public Server {
public:
    AgentServer();
    ~AgentServer();

    virtual std::string get_lua_file();

    virtual Manager* create_manager();
};


#endif