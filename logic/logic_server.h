#ifndef _LOGIC_SERVER_H_
#define _LOGIC_SERVER_H_


#include <server.h>


class LogicServer : public Server {
public:
    LogicServer();
    virtual ~LogicServer();

    virtual std::string get_lua_file();

    virtual Manager* create_manager();

};

#endif
