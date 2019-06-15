#include "server.h"
#include <signal.h>

#include "utils/logger.h"
#include "utils/lua_manager.h"
#include "utils/function.h"


void exports_logger(lua_State* L);
void exports_hub(lua_State* L);
void exports_redis(lua_State* L);
void exports_utils(lua_State* L);


Server::Server():m_redis(&m_hub), m_signal(&m_hub)
{

}


Server::~Server() = default;


int Server::export_lua(lua_State* L)
{
    exports_logger(L);
    exports_hub(L);
    exports_redis(L);
    exports_utils(L);
}


int Server::init()
{
    m_manager = this->create_manager();

    // load lua
    LuaManager::get_inst()->init_lua();

    this->export_lua(LL);

    luabridge::LuaRef tc = luabridge::getGlobal(LL, "tc");
    tc["hub"] = &m_hub;
    tc["redis"] = &m_redis;

    LuaManager::get_inst()->do_file(this->get_lua_file().c_str());

    // 根据 id 返回 redis
    luabridge::LuaRef fn_get_redis_conf = luabridge::getGlobal(LL, "get_redis_conf");
    if(!fn_get_redis_conf.isFunction()){
        gl_error("lua file no get_redis_conf().");
        return -1;
    }

    luabridge::LuaRef redis_conf = fn_get_redis_conf(m_id);
    if(!redis_conf.isTable()){
        gl_error("redis_conf error.");
        return -1;
    }
    std::string rds_host = redis_conf[1].cast<std::string>();
    int rds_port = redis_conf[2].cast<int>();
    

    gl_trace(sf("redis host=%s, port=%d, auth=%d",
        rds_host.c_str(), rds_port, !redis_conf[3].isNil()).c_str());

    m_redis.connect(rds_host.c_str(), rds_port);
    if(!redis_conf[3].isNil()){
        std::string rds_auth = redis_conf[3].cast<std::string>();
        m_redis.exec(sf("auth %s", rds_auth.c_str()).c_str(), nullptr);
    }
    
    // read redis address, connect

    // read zookeeper address
    // 1, connect
    // 2, read all other server address and set watchers on them
    // 3, connect all servers 
    return 0;
}


int Server::run(int argc, char** argv)
{
    check_boot_params(argc, argv);

    std::string log_name = string_format("%s_%d", argv[0], m_id);

    int ec = 0;
#ifdef _DEBUG
    LOG->init(log_name.c_str(), LOGGER_LEVEL_TRACE);
#else
    LOG->init(log_name.c_str(), LOGGER_LEVEL_TRACE);
#endif

    ec = this->init();

    m_signal.start(SIGINT, [this](int sig){
        gl_info("user quit.");
        this->m_redis.close();
        this->m_hub.stop();
        LOG->stop();

        exit(0);
    });

    ec = m_hub.serve_forever();

    return ec;
}


bool Server::check_boot_params(int argc, char** argv)
{
    if(argc < 2){
        cout << "Usage:" << endl;
        cout << argv[0] << " " << "[agent_id]" << endl
            << "where 'agent_id' should be a number and greater than 0."
            << endl;

        exit(0);
    }

    int32_t id = atoi(argv[1]);

    if( id <= 0 ){
        cout << ("bad argument [id].") << endl;
        exit(0);
    }

    m_id = id;

    return true;
}

Manager* Server::create_manager()
{
    return nullptr;
}