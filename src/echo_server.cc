#include <session.h>
#include <listener.h>
#include <logger.h>
#include <session.h>
#include <iostream>
#include <uv.h>

class EchoDispatcher : public IDispatcher{
public:
    virtual void dispatch(ISession* ses, char* data, int data_len){
        std::cout << data << data_len << std::endl;
    }
};

class SManager : public Manager{
public:
    SManager(Hub* b):Manager(b){
    }
    virtual ISession* new_session(){
        std::cout<<"new new_session" << std::endl;
        return new ISession(this->m_hub, new EchoDispatcher(), this);
    }
    virtual void free_session(ISession* ses){
        delete ses;
        ses = 0;
    }
};

int main(){
    Hub h;
    SManager m(&h);
    Listener l(&h,&m);
    int ec = l.listen("127.0.0.1", 4040);
    if(ec){
        
        std::cout<< ec << ":" << uv_err_name(ec) << std::endl;
    }

    h.serve_forever();

    return 0;
}