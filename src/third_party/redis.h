#ifndef _THECURE_REDIS_H_
#define _THECURE_REDIS_H_

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>

/** Redis interface
 * 
 * support 
 * 1) async and sync API
 * 2) auto reconnect
 * 
 */
class AsyncRedis {
public:
    AsyncRedis();
    ~AsyncRedis();

    void connect();
    void close();

    void exec();
    void exec_l();// export to lua
protected:
    static void on_connected();
    static void on_disconnected();
    static void on_command();

protected:
    redisAsyncContext* m_ctx;
};


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
