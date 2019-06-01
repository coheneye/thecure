#include <assert.h>
#include "listener.h"
#include "session.h"
#include "manager.h"
#include <uv.h>
#include <iostream>

Listener::Listener(Hub* s, Manager* m):m_hub(s),m_mgr(m)
{
    m_hdl = malloc(sizeof(uv_tcp_t));
    uv_tcp_t* tmp = (uv_tcp_t*)m_hdl;
    tmp->data = (void*)this;

    int rc = uv_tcp_init((uv_loop_t*)s->handle(), tmp);
    assert(rc == 0);
}


Listener::~Listener()
{
    auto on_closed = [](uv_handle_t* h){

    };
    uv_close((uv_handle_t*)m_hdl, on_closed);
    free(m_hdl);
    m_hdl = 0;
}


int Listener::listen(const char* ip, unsigned short port)
{
    int ec = 0;
    sockaddr_in addr;

    ec = uv_ip4_addr(ip, port, &addr);
    if(ec){
        std::cout<< 1 << std::endl;
        return ec;
    }

    ec = uv_tcp_bind((uv_tcp_t*)m_hdl, (const sockaddr*)&addr, 0);
    if(ec){
        std::cout<< 2 << std::endl;
        return ec;
    }

    auto on_new_conn = [](uv_stream_t * s, int status){
        if(status < 0){
            //TODO: error
            std::cout << "on_new_conn error" << std::endl;
            return;
        }

        Listener* lis = (Listener*)s->data;
        int ret = lis->m_mgr->do_accept((void*)s);
        if (ret){
            //TODO: error
            std::cout<< "do_aacept return failed" << std::endl;
        }
    };

    return uv_listen((uv_stream_t*)m_hdl, 1024, on_new_conn);
}
