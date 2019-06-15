#ifndef _THECURE_SERVER_H_
#define _THECURE_SERVER_H_


#include "hub/hub.h"
#include "utils/luacxx.h"
#include "hub/manager.h"
#include "hub/redis.h"
#include "hub/signal.h"


class Server {
public:
    Server();

    virtual ~Server();

    virtual int init();

    virtual int export_lua(lua_State* L);

    virtual std::string get_lua_file() = 0;

    virtual int run(int argc, char** argv);

    virtual bool check_boot_params(int argc, char** argv);

    virtual Manager* create_manager();

protected:

    int32_t m_id;
    Hub m_hub;
    Signal m_signal;
    Manager* m_manager;
    AsyncRedis m_redis;
};


#endif