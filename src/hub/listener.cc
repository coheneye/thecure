#include <assert.h>
#include "listener.h"
#include "session.h"
#include "manager.h"


Listener::Listener(Server* s):m_server(s)
{
    uv_tcp_init(s->handle(), &m_hot);
    m_hot.data = (void*)this;
}


Listener::~Listener()
{
    auto on_closed = [](uv_handle_t* h){

    };

    uv_close((uv_handle_t*)&m_hot, on_closed);
}


int Listener::listen(const char* ip, unsigned short port)
{
    int ec = 0;
    sockaddr_in addr;

    ec = uv_ip4_addr(ip, port, &addr);
    if(ec){
        return ec;
    }

    ec = uv_tcp_bind(&m_hot, (const sockaddr*)&addr, 0);
    if(ec){
        return ec;
    }

    auto on_new_conn = [](uv_stream_t * s, int status){
        if(status < 0){
            return;
        }

        Listener *l = (Listener*)s->data;
        // free session instance sometime somewhere
        Session *ses = new Session(l->m_server);
        if(!ses){
            //log
            return;
        }
    
        if(ses->accept(s)){
            delete ses;
            return;
        }
        ses->start_read();
    };

    return uv_listen((uv_stream_t*)&m_hot, 1024, on_new_conn);
}
