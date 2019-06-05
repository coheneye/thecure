#include <hub/session.h>
#include <hub/listener.h>
#include <utils/logger.h>
#include <hub/session.h>
#include <iostream>
#include <uv.h>


class ClientDispatcher : public IDispatcher{
public:
    virtual void dispatch(ISession* ses, char* data, int data_len){
        std::cout << data << data_len << std::endl;
    }
private:
};

class ServerDispatcher : public IDispatcher{
public:
    virtual void dispatch(ISession* ses, char* data, int data_len){
        std::cout << data << data_len << std::endl;
    }
private:   
};

class SManager : public Manager{
public:
    SManager(Hub* b):Manager(b){

    }
    virtual ISession* new_session(){
        std::cout<<"new new_session" << std::endl;
        return new ISession(this->m_hub, new ClientDispatcher(), this);
    }
    virtual void free_session(ISession* ses){
        delete ses->m_disp;
        delete ses;
        ses = 0;
    }
};


class CManager : public Manager{
public:
    CManager(Hub* h):Manager(h){

    }
    virtual ISession* new_session(){
        std::cout<<"new new_session" << std::endl;
        return new ISession(this->m_hub, new ServerDispatcher(), this);
    }
    virtual void free_session(ISession* ses){
        delete ses->m_disp;
        delete ses;
        ses = 0;
    } 
};

void test_defer(){
    defer _(nullptr, [](...){std::cout<<"dddd" << std::endl; });
    defer a(nullptr, [](...){std::cout << "eeee" << std::endl; });
    std::cout << "HHHH" << std::endl;
}


/** agent
 * 启动参数：agent_id 隐含参数 agent/main.lua
 * 
 * 启动后读取 main.lua 中配置，链接 zookeeper, redis, login server, event server 
 * 注册自身到 zookeeper 
 * 读取 zookeeper 
 */
int main(){
    test_defer();
    Hub h;
    SManager m(&h);
    CManager cm(&h);
    ISession * ses = cm.new_session();
    ses->connect("", 2121);
    ses->set_tag((void*)111);

    Listener l(&h,&m);
    int ec = l.listen("127.0.0.1", 4040);
    if(ec){
        
        std::cout<< ec << ":" << uv_err_name(ec) << std::endl;
    }

    h.serve_forever();

    return 0;
}