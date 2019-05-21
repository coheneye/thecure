#ifndef _THECURE_REDIS_H_
#define _THECURE_REDIS_H_

#include "server.h"
#include <hiredis/async.h>

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

    int exec(const char* command);
    void exec_l();// export to lua
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
