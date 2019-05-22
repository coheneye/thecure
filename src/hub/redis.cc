#include "redis.h"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>
#include <logger.h>


AsyncRedis::AsyncRedis(Server* s):m_server(s), m_closing(false)
{

}

AsyncRedis::~AsyncRedis()
{
    this->close();
}

int AsyncRedis::connect(const char* ip, int port)
{
    m_closing = false;
    m_ctx = redisAsyncConnect(ip, port);
    if(m_ctx->err){
        LOG->err("connect redis");
        return m_ctx->err;
    }

    int err = redisLibuvAttach(m_ctx, m_server->handle());
    if(err){
        LOG->err("attach error");
        return err;
    }

    err = redisAsyncSetConnectCallback(m_ctx, on_connected);
    if(err){
        LOG->err("set connect callback failed");
        return err;
    }

    err = redisAsyncSetDisconnectCallback(m_ctx, on_disconnected);
    if(err){
        LOG->err("set disconnect callback failed");
        return err;
    }

    return REDIS_OK;
}

void AsyncRedis::close()
{
    m_closing = true;

    redisAsyncDisconnect(m_ctx);
    redisAsyncFree(m_ctx);
}


int AsyncRedis::exec(const char* command)
{
    int err = redisAsyncCommand(m_ctx, on_command, (void*)this, command);
    if(err){
        LOG->err("execute redis command failed");
    }
    return err;
}

void AsyncRedis::exec_l()// export to lua
{

}


void AsyncRedis::on_connected(const redisAsyncContext* c, int status)
{
    if(status != REDIS_OK){
        LOG->cri("connect redis failed.");
        return;
    }
    LOG->inf("connect redis success.");
}


void AsyncRedis::on_disconnected(const redisAsyncContext* c, int status)
{
    if(status != REDIS_OK){
        LOG->cri("redis lost connection");
        return;
    }
}


void AsyncRedis::on_command(redisAsyncContext* c, void* r, void* privdate)
{
    redisReply* reply = (redisReply*)r;
    if(!reply){
        LOG->err("redis reply with nullptr");
        return;
    }
    AsyncRedis* that = (AsyncRedis*)privdata;
    if(!that){
        LOG->err("redis privdata is nullptr");
        return;
    }
    
    // real callback
}

