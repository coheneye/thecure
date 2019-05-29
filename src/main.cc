extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include <iostream>
#include <unistd.h>
#include <logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>

//in lua
void 
jprint(const char* content){
    //LOG->inf(content);
}

// in lua
class 
Line{
public:
    Line():m_data(""){}
    Line(const std::string& data):m_data(data){}

    std::size_t length() const {
        return m_data.length();
    }

protected:
    std::string m_data;
};

template<class T>
class Mgr{
public:

private:
};
// create logger with 2 targets with different log levels and formats.
// the console will show only warnings or errors, while the file will log all.
void multi_sink_example()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("multisink.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(spdlog::level::debug);
    logger.debug("what");
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");
}

void multi_sink_example2()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    stdout_sink->set_level(spdlog::level::trace);

    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("mylog.txt", 1024*1024*10, 3);
    rotating_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::async_logger>("loggername", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    logger->set_level(spdlog::level::debug);
    
    spdlog::register_logger(logger);
    logger->debug("good");
    logger->info("waht");
}

// main
int
main(int argc, char** argv)
{
    //multi_sink_example2();
    //multi_sink_example();
    // init logger
    LOG->init(LOGGER_LEVEL_TRACE);

    LOG->dbg("init lua");
    LOG->inf("export to lua");
    LOG->err("take care");

    // spdlog::set_level(spdlog::level::trace);
    // spdlog::debug("debug"); 
    // spdlog::trace("trac");

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Line>("Line")
            .addConstructor<void(*)(const std::string&)>()
            .addFunction("get_length", &Line::length)
        .endClass()
        .addFunction("jprint", jprint);
    
    const char* lua_code = " \
        local line = Line('test') \n \
        jprint(tostring(line:get_length()))\n \
    ";
    // luaL_loadfile(L, "main.lua");
    luaL_dostring(L, lua_code);

    //log->log_dbg("Hello");
    // say this is an event loop
    // while(1){
    //     sleep(1);
    // }
    
    lua_close(L);

    sleep(1);

    return 0;
}
