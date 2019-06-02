#ifndef _THECURE_REDIS_H_
#define _THECURE_REDIS_H_

#include <unordered_map>
#include <functional>
#include "hub.h"
#include <hiredis/async.h>
#include "../utils/luacxx.h"
#include "../LuaBridge/LuaBridge.h"

/** Redis interface
 * 
 * support 
 * 1) async connect and command
 * 2) auto reconnect
 * todo: batched command
 */

typedef std::function<void(redisReply*)> cb_command_t;

class AsyncRedis final {
public:
    AsyncRedis(Hub* s);
    ~AsyncRedis();

    int connect(const char* ip, int port);
    void close();

    // in c++, we save the callback func address, then call it when reply come back.
    int exec(const char* command, cb_command_t func);
    // in lua manner, we put every callback function into a map, indexed with hash of command
    // and then call the function which request the command.
    void exec_l(lua_State* L, const char* command, luabridge::LuaRef ref);// export to lua
protected:
    static void on_connected(const redisAsyncContext* c, int status);
    static void on_disconnected(const redisAsyncContext* c, int status);
    static void on_command(redisAsyncContext* c, void* r, void* privdate);

protected:
    std::unordered_map<std::string, std::string> m_callbacks;
    redisAsyncContext* m_ctx;
    Hub* m_hub;
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
