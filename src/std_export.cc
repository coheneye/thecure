#include "utils/logger.h"
#include "utils/lua_manager.h"
#include "utils/function.h"
#include <utils/crypto.h>
#include <utils/time.h>

#include <hub/timer.h>
#include <hub/task.h>
#include <hub/redis.h>

#include <LuaBridge/LuaBridge.h>
using namespace luabridge;


lua_Debug get_lua_running_pos()
{
    lua_Debug ar;
    lua_State* L = LL;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "Sln", &ar);
    return ar;
}

static void trace(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->trc(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}

static void debug(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->dbg(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}

static void info(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->inf(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}

static void warn(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->wrn(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}

static void error(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->err(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}

static void critical(const char* s)
{
    lua_Debug ar = get_lua_running_pos();
    LOG->cri(s, get_file_name(ar.short_src), ar.currentline, ar.name);
}


void exports_logger(lua_State* L)
{
    getGlobalNamespace(L)
        .beginNamespace("log")
            .addFunction("trace", trace)
            .addFunction("debug", debug)
            .addFunction("info", info)
            .addFunction("warn", warn)
            .addFunction("error", error)
            .addFunction("critical", critical)
        .endNamespace();
}

void exports_hub(lua_State* L)
{
    getGlobalNamespace(L)
        .beginNamespace("tc")
            .beginClass<Hub>("hub")
            .endClass()
            .beginClass<Timer>("timer")
                .addConstructor<void (*) (Hub*)>()
                .addFunction("start", &Timer::start_lua)
                .addFunction("stop", &Timer::stop)
                .addFunction("again", &Timer::again)
                .addFunction("set_interval", &Timer::set_interval)
                .addFunction("get_interval", &Timer::get_interval)
                .addFunction("is_active", &Timer::is_active)
            .endClass()
            .beginClass<Task>("task")
                .addConstructor<void (*) (Hub*)>()
                .addFunction("run", &Task::run_lua)
                .addFunction("cancel", &Task::cancel)
                .addFunction("send", &Task::send_lua)
            .endClass()
        .endNamespace();
}


void exports_redis(lua_State* L)
{
    getGlobalNamespace(L)
        .beginNamespace("tc")
            .beginClass<RedisResult>("redis_res")   //no constructor, we should never create its instance in lua
                .addFunction("type", &RedisResult::type)
                .addFunction("array_size", &RedisResult::array_size)
                .addFunction("value_of_int", &RedisResult::value_of_int)
                .addFunction("value_of_str", &RedisResult::value_of_str)
                .addFunction("value_of_arr", &RedisResult::value_of_arr)
            .endClass()
            .beginClass<AsyncRedis>("redis")
            //    .addConstructor<void (*) (Hub*)>()
            //    .addFunction("connect", &AsyncRedis::connect)
            //    .addFunction("close", &AsyncRedis::close)
                .addFunction("is_connected", &AsyncRedis::is_connected)
                .addFunction("exec", &AsyncRedis::exec_lua)
            .endClass()
        .endNamespace();
}


void exports_utils(lua_State* L)
{
    getGlobalNamespace(L)
        .beginNamespace("utils")
            .beginNamespace("crypto")
                .addFunction("b64enc", &crypto::b64enc)
                .addFunction("b64dec", &crypto::b64dec)
                .addFunction("gen_nonce", &crypto::gen_nonce)
                .addFunction("get_hash_key", &crypto::gen_key_hash)
                .addFunction("get_hmac256_key", &crypto::gen_key_hash_hmac256)
                .addFunction("get_hmac512_key", &crypto::gen_key_hash_hmac512)
                .addFunction("sha256", &crypto::hash_sha256)
                .addFunction("sha512", &crypto::hash_sha512)
                .addFunction("hmac256",&crypto::hash_hmac256)
                .addFunction("hmac512",&crypto::hash_hmac512)
                .addFunction("hash", &crypto::hash)
                .addFunction("random", &crypto::random)
                .addFunction("random_uniform", &crypto::random_uniform)
                .addFunction("random_str", &crypto::random_str)
            .endNamespace()
            .beginNamespace("time")
                .addFunction("now_in_sec", &now_in_second)
                .addFunction("now_in_mill", &now_in_millsecond)
                .addFunction("now_in_str", &now_in_str)
                .addFunction("today_in_str", &today_in_str)
            .endNamespace()
            .beginClass<TimeCounter>("time_counter")
                .addConstructor<void(*)(void)>()
                .addFunction("elapse_secs", &TimeCounter::elapse_secs)
                .addFunction("elapse_mills", &TimeCounter::elapse_mills)
                .addFunction("reset", &TimeCounter::reset)
            .endClass()
        .endNamespace();
}
