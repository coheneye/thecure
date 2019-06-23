#include "server.h"
#include <signal.h>
#include <cjson/cJSON.h>

#include "utils/logger.h"
#include "utils/lua_manager.h"
#include "utils/function.h"


// implements in std_export.cc
void exports_logger(lua_State* L);
void exports_hub(lua_State* L);
void exports_redis(lua_State* L);
void exports_utils(lua_State* L);

typedef struct _conf {
    std::string host, auth;
    int port;   
} conf_t ;


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
    tc["zk"] = &m_zk;

    LuaManager::get_inst()->do_file(this->get_lua_file().c_str());
    int ec = this->connect_to_zk();
    if(0 != ec){
        return ec;
    }
    // read redis address, connect
    ec = this->connect_to_redis();

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

int Server::connect_to_redis()
{
    std::string js_redis_conf;
    utility::zoo_rc rc = m_zk.get_node("/thecure/redis", js_redis_conf, nullptr, false);
    if(utility::z_ok != rc){
        gl_error("read redis conf from zk failed");
        return rc;
    }
    cJSON* items = cJSON_Parse(js_redis_conf.c_str());
    if(cJSON_IsInvalid(items) || !cJSON_IsArray(items)){
        gl_error("there is no redis config");
        return -1;
    }
    int size = cJSON_GetArraySize(items);
    if(0 >= size){
        gl_error("there is a blank redis config");
        return -1;
    }
    // 1st
    std::vector<conf_t> confs;
    cJSON* item;
    cJSON_ArrayForEach(item, items)
    {
        conf_t conf;
        cJSON* host = cJSON_GetObjectItemCaseSensitive(item, "ip");
        cJSON* port = cJSON_GetObjectItemCaseSensitive(item, "port");
        cJSON* auth = cJSON_GetObjectItemCaseSensitive(item, "auth");
        if(!cJSON_IsString(host) || !cJSON_IsNumber(port) || !cJSON_IsString(auth)){
            return -1;
        }
        conf.host = host->valuestring;
        conf.port = port->valueint;
        conf.auth = host->valuestring;

        confs.push_back(conf);
    }

    if(confs.size() > 0){
        m_redis.connect(confs[0].host.c_str(), confs[0].port);
        if(confs[0].auth.length() > 0){
            m_redis.exec(sf("auth %s", confs[0].auth.c_str()).c_str(), nullptr);
        }
    }else{
        gl_error("redis config error");
        return -1;
    }
    return 0;
}

int Server::connect_to_zk()
{
    luabridge::LuaRef fn_get_zk_conf = luabridge::getGlobal(LL, "get_zk_conf");
    if(!fn_get_zk_conf.isFunction()){
        gl_error("lua file no get_zk_conf().");
        return -1;
    }
    luabridge::LuaRef zk_conf = fn_get_zk_conf();
    if(!zk_conf.isTable()){
        gl_error("zk conf error.");
        return -1;
    }
    std::string url = zk_conf[1].cast<std::string>();
    utility::zoo_rc r = m_zk.connect(url);
    if(r != utility::z_ok){
        gl_error("zk connect error.");
        return r;
    }

    return r;
}

Manager* Server::create_manager()
{
    return nullptr;
}