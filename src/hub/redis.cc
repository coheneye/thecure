#include <hub/redis.h>

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libuv.h>


AsyncRedis::AsyncRedis(Hub* s):m_hub(s), m_closing(false)
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
        return m_ctx->err;
    }

    int err = redisLibuvAttach(m_ctx, (uv_loop_t*)(m_hub->handle()));
    if(err){
        return err;
    }

    err = redisAsyncSetConnectCallback(m_ctx, on_connected);
    if(err){
        return err;
    }

    err = redisAsyncSetDisconnectCallback(m_ctx, on_disconnected);
    if(err){
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


int AsyncRedis::exec(const char* command, cb_command_t func)
{
    int err = redisAsyncCommand(m_ctx, on_command, (void*)&func, command);
    if(err){
    }
    return err;
}

void AsyncRedis::exec_l(lua_State* L, const char* command, luabridge::LuaRef ref)// export to lua
{

}


void AsyncRedis::on_connected(const redisAsyncContext* c, int status)
{
    if(status != REDIS_OK){
        return;
    }
}


void AsyncRedis::on_disconnected(const redisAsyncContext* c, int status)
{
    if(status != REDIS_OK){
        return;
    }
}


void AsyncRedis::on_command(redisAsyncContext* c, void* r, void* privdata)
{
    redisReply* reply = (redisReply*)r;
    if(!reply){
        return;
    }
    cb_command_t *func = (cb_command_t*)privdata;
    if(!func){
        return;
    }
    (*func)(reply);
}

