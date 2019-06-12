#include <hub/session.h>
#include <hub/listener.h>
#include <hub/redis.h>
#include <hub/timer.h>
#include <hub/idle.h>
#include <hub/task.h>
#include <signal.h>
#include <hub/signal.h>

#include <utils/logger.h>
#include <utils/function.h>

#include <iostream>
#include "exports.h"
#include <utils/lua_manager.h>


/**
 *  检查进程启动参数，如果正确返回启动的ID参数，失败则直接结束进程
 * 
 */
int check_boot_params(int argc, char** argv);

/** agent
 * 启动参数：agent_id 隐含参数 agent/main.lua
 * 
 * 启动后读取 main.lua 中配置，链接 zookeeper, 
 * 注册自身到 zookeeper 
 * 读取 zookeeper redis, login server, event server 
 */
int main(int argc, char** argv)
{
    auto id = check_boot_params(argc, argv);

    std::string log_name = string_format("%s_%d", argv[0], id);

    LOG->init(log_name.c_str(), LOGGER_LEVEL_TRACE);
    
    //gl_info("good, very good");

    Hub h;

    LuaManager::get_inst()->init_lua();

    exports(LL);

    AsyncRedis redis(&h);
    redis.connect("127.0.0.1", 6379);

    luabridge::LuaRef tc = luabridge::getGlobal(LL, "tc");
    tc["hub"] = &h;
    tc["redis"] = &redis;

    LuaManager::get_inst()->do_file("./lua/main.lua");
  
    Signal signal(&h);
    signal.start(SIGINT, [&h, &redis](int sig){
        gl_info("user quit.");
        h.stop();
        redis.close();
        LOG->stop();
        exit(0);
    });
    
    h.serve_forever();
    
    return 0;
}

int check_boot_params(int argc, char** argv)
{
    if(argc < 2){
        cout << "Usage:" << endl;
        cout << argv[0] << " " << "[agent_id]" << endl
            << "where 'agent_id' should be a number and greater than 0."
            << endl;

        exit(0);
    }

    int id = atoi(argv[1]);
    if( id <= 0 ){
        gl_error("bad argument [id].");
        exit(0);
    }

    return id;
}