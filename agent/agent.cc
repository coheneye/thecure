#include "agent_server.h"


/** agent
 * 启动参数：agent_id 隐含参数 agent/main.lua
 * 
 * 启动后读取 main.lua 中配置，链接 zookeeper, 
 * 注册自身到 zookeeper 
 * 读取 zookeeper redis, login server, event server 
 */
int main(int argc, char** argv)
{
    AgentServer as;

    as.run(argc, argv);

    return 0;
}

