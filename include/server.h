#ifndef _THECURE_SERVER_H_
#define _THECURE_SERVER_H_


#include "hub/hub.h"
#include "utils/luacxx.h"
#include "hub/manager.h"


class Server{
public:
    Server();
    virtual ~Server();

    virtual int init();
    virtual int export_lua(lua_State* L) = 0;
    virtual int get_lua_file() = 0;
    virtual int run();

protected:
    Hub m_hub;
};


#endif