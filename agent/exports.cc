#include "exports.h"
#include <utils/function.h>
#include <utils/lua_manager.h>
#include <utils/logger.h>
#include <hub/timer.h>

#include <LuaBridge/LuaBridge.h>
using namespace luabridge;


lua_Debug get_lua_running_pos()
{
    lua_Debug ar;
    lua_State* L = LuaManager::get_inst()->get_state();
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
        .endNamespace();
}


bool exports(lua_State* L)
{
    exports_logger(L);
    exports_hub(L);
    return true;
}