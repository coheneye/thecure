#include <hub/redis.h>
#include <utils/function.h>
#include <utils/logger.h>

#include <hiredis/adapters/libuv.h>


RedisResult::RedisResult(redisReply* r):m_reply(r)
{

}
    
RedisResult::~RedisResult()=default;

int RedisResult::array_size(){
    if(!m_reply){
        return 0;
    }
    return m_reply->elements;
}

RedisResult RedisResult::value_of_arr(int i)
{
    if(!m_reply){
        return RedisResult(nullptr);
    }
    if(i < 0 || i > m_reply->elements || m_reply->type != REDIS_REPLY_ARRAY){
        return RedisResult(nullptr);
    }
    return RedisResult(m_reply->element[i]);
}

/**
 #define REDIS_REPLY_STRING 1
 #define REDIS_REPLY_ARRAY 2
 #define REDIS_REPLY_INTEGER 3
 #define REDIS_REPLY_NIL 4
 #define REDIS_REPLY_STATUS 5
 #define REDIS_REPLY_ERROR 6
 */
int RedisResult::type()
{
    if(!m_reply){
        return REDIS_REPLY_NIL;
    }
    return m_reply->type;
}

uint64_t RedisResult::value_of_int()
{
    if(!m_reply){
        return 0;
    }
    return m_reply->integer;
}

char* RedisResult::value_of_str()
{
    if(!m_reply){
        return nullptr;
    }
    return m_reply->str;
}



// ================================= AsyncRedis ================================

AsyncRedis::AsyncRedis(Hub* s):m_hub(s), m_closing(false), m_is_connected(false), m_timer_reconnnect(s), m_ctx(nullptr)
{

}


AsyncRedis::~AsyncRedis()
{
    this->close();
}


int AsyncRedis::connect(const char* ip, int port)
{
    m_host = ip;
    m_port = port;
    return this->reconnect();
}

int AsyncRedis::reconnect()
{
    m_closing = false;

    if(m_ctx){
        redisAsyncFree(m_ctx);
        m_ctx = nullptr;
    }

    m_ctx = redisAsyncConnect(m_host.c_str(), m_port);
    if(!m_ctx){
        gl_crit("redis context is nullptr.");
        return -1;
    }

    if(m_ctx->err){
        gl_crit(sf("redis context error: %d", m_ctx->err).c_str());
        return m_ctx->err;
    }

    m_ctx->data = (void*)this;

    int err = redisLibuvAttach(m_ctx, (uv_loop_t*)(m_hub->handle()));
    if(err){
        gl_crit(sf("redis redisLibuvAttach error: %d", err).c_str());
        return err;
    }

    err = redisAsyncSetConnectCallback(m_ctx, &AsyncRedis::on_connected);
    if(err){
        gl_crit(sf("redis redisAsyncSetConnectCallback error: %d", err).c_str());
        return err;
    }

    err = redisAsyncSetDisconnectCallback(m_ctx, &AsyncRedis::on_disconnected);
    if(err){
        gl_crit(sf("redis redisAsyncSetDisconnectCallback error: %d", err).c_str());
        return err;
    }

    return err;
}

void AsyncRedis::close()
{
    gl_debug("closing redis");

    m_closing = true;

    redisAsyncDisconnect(m_ctx);
    redisAsyncFree(m_ctx);
}

bool AsyncRedis::is_connected()
{
    return m_is_connected;
}

int AsyncRedis::exec(const char* command, cb_command_t func)
{
    m_callbacks.insert(pair<string, cb_command_t>(command, func));

    auto it = m_callbacks.upper_bound(command);
    it--;

    int ec = redisAsyncCommand(m_ctx, &AsyncRedis::on_command, (void*)(it->first.c_str()), command);
    if(ec){
        gl_error(sf("redis exec() return error: %d", ec).c_str());
        m_callbacks.erase(it);
    }

    return ec;
}


/**
 * 
 */
int AsyncRedis::exec_lua(const char* command, luabridge::LuaRef cb)// export to lua
{
    m_callbacks_lua.insert(pair<string, LuaRef>(command, cb));

    auto it = m_callbacks_lua.upper_bound(command);
    it--;

    int ec = redisAsyncCommand(m_ctx, on_lua_command, (void*)(it->first.c_str()), command);
    if(ec){
        gl_warn("redis exec_lua() return error");
        m_callbacks_lua.erase(it);
    }
    return ec;
}


void AsyncRedis::on_connected(const struct redisAsyncContext* c, int status)
{
    if(status != REDIS_OK){
        gl_crit(sf("redis connect failed: %d", status).c_str());
        return;
    }
    
    AsyncRedis* that = (AsyncRedis*)c->data;
    that->m_is_connected = true;

    if(that->m_timer_reconnnect.is_active()){
        that->m_timer_reconnnect.stop();
    }
    gl_info("redis connected.");
}


void AsyncRedis::on_disconnected(const struct redisAsyncContext* c, int status)
{
    AsyncRedis* that = (AsyncRedis*)c->data;
    that->m_is_connected = false;

    if(status != REDIS_OK){
        gl_crit(sf("redis disconnected and error: %d", status).c_str());
        return;
    }
    gl_error("redis disconnected");

    if(!that->m_closing){
        that->m_timer_reconnnect.start(1000, 5000, [that](void* p){
            that->reconnect();
            gl_warn("redis connect retrying ... ");
        }, nullptr);
    }
}


void AsyncRedis::on_command(redisAsyncContext* c, void* r, void* privdata)
{
    redisReply* reply = (redisReply*)r;
    if(!reply){
        gl_error("redis reply null.");
        return;
    }
    defer _(nullptr, [reply](...){
        freeReplyObject(reply);
    });

    AsyncRedis* that = (AsyncRedis*)c->data;
    std::string cmd((char*)privdata);
    auto it = that->m_callbacks.lower_bound(cmd);
    if(it != that->m_callbacks.end()){
        if(it->second){
            RedisResult r = RedisResult(reply);
            it->second(r);
        }
        // }else{
        //     gl_warn(sf("redis cmd[%s] without a callback", cmd.c_str()).c_str());
        // }
        
        that->m_callbacks.erase(it);
    }else{
        gl_error(sf("redis cmd[%s] not found.", cmd.c_str()).c_str());
    }
}


void AsyncRedis::on_lua_command(redisAsyncContext* c, void* r, void* privdata)
{
    redisReply* reply = (redisReply*)r;
    if(!reply){
        gl_error("redis reply null.");
        return;
    }

    defer _(nullptr, [reply](...){
        freeReplyObject(reply);
    });

    AsyncRedis* that = (AsyncRedis*)c->data;
    std::string cmd((char*)privdata);
    auto it = that->m_callbacks_lua.lower_bound(cmd);
    if(it != that->m_callbacks_lua.end()){
        if(it->second.isFunction()){
            RedisResult r = RedisResult(reply);
            it->second(&r);
        }
        that->m_callbacks_lua.erase(it);
    }else{
        gl_error(sf("redis lua cmd not found[cmd=%s]", cmd.c_str()).c_str());
    }
}
