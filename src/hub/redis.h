#ifndef _THECURE_REDIS_H_
#define _THECURE_REDIS_H_

#include "server.h"
#include <hiredis/async.h>
#include <luacxx.h>
#include <LuaBridge/LuaBridge.h>

/** Redis interface
 * 
 * support 
 * 1) async connect and command
 * 2) auto reconnect
 * todo: batched command
 */
class AsyncRedis final {
public:
    AsyncRedis(Server* s);
    ~AsyncRedis();

    int connect(const char* ip, int port);
    void close();

    // in c++, we save the callback func address, then call it when reply come back.
    int exec(const char* command);
    // in lua manner, we put every callback function into a map, indexed with hash of command
    // and then call the function which request the command.
    void exec_l(lua_State* L, const char* command, luabridge::LuaRef ref);// export to lua
protected:
    static void on_connected(const redisAsyncContext* c, int status);
    static void on_disconnected(const redisAsyncContext* c, int status);
    static void on_command(redisAsyncContext* c, void* r, void* privdate);

protected:
    redisAsyncContext* m_ctx;
    Server* m_server;
    bool m_closing;  // intend to close
};


/**
 * connect redis synchronously
 * do not implement now.
 */
class SyncRedis {
public:
    SyncRedis();
    ~SyncRedis();

    void connect();
    void close();

    void exec();
protected:
    redisContext* m_ctx;
};


#endif
