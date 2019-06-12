#ifndef _THECURE_REDIS_H_
#define _THECURE_REDIS_H_

#include <map>
#include <functional>
#include "hub.h"
#include "timer.h"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <utils/luacxx.h>
#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

/** Redis interface
 * 
 * support 
 * 1) async connect and command
 * 2) auto reconnect
 * todo: batched command
 */

class RedisResult{
public:
    explicit RedisResult(redisReply* r);
    ~RedisResult();

    int array_size();

    RedisResult value_of_arr(int i);

    /**
     #define REDIS_REPLY_STRING 1
     #define REDIS_REPLY_ARRAY 2
     #define REDIS_REPLY_INTEGER 3
     #define REDIS_REPLY_NIL 4
     #define REDIS_REPLY_STATUS 5
     #define REDIS_REPLY_ERROR 6
     */
    int type();

    uint64_t value_of_int();

    char* value_of_str();

protected:
    redisReply* m_reply;
};


typedef std::function<void(RedisResult&)> cb_command_t;

class AsyncRedis final {
public:
    AsyncRedis(Hub* s);
    ~AsyncRedis();

    int connect(const char* ip, int port);

    int reconnect();
    
    void close();

    bool is_connected();
    
    int exec(const char* command, cb_command_t func);
    int exec_lua(const char* command, luabridge::LuaRef ref);// export to lua
protected:
    static void on_connected(const struct redisAsyncContext* c, int status);
    static void on_disconnected(const struct redisAsyncContext* c, int status);
    static void on_command(struct redisAsyncContext* c, void* r, void* privdate);
    static void on_lua_command(struct redisAsyncContext* c, void* r, void* privdata);

protected:
    std::multimap<std::string, cb_command_t> m_callbacks;
    std::multimap<std::string, LuaRef> m_callbacks_lua;

    redisAsyncContext* m_ctx;
    Hub* m_hub;
    Timer m_timer_reconnnect;
    std::string m_host;
    int m_port;
    bool m_closing;  // intend to close
    bool m_is_connected;
};


#endif
