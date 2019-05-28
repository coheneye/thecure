#include <assert.h>
#include "listener.h"
#include "session.h"
#include "manager.h"
#include <uv.h>


Listener::Listener(Hub* s, Manager* m):m_hub(s),m_mgr(m)
{
    m_hot = malloc(sizeof(uv_tcp_t));
    uv_tcp_t* tmp = (uv_tcp_t*)m_hot;
    tmp->data = (void*)this;

    uv_tcp_init((uv_loop_t*)s->handle(), (uv_tcp_t*)m_hot);
}


Listener::~Listener()
{
    auto on_closed = [](uv_handle_t* h){

    };
    uv_close((uv_handle_t*)&m_hot, on_closed);
    free(m_hot);
    m_hot = 0;
}


int Listener::listen(const char* ip, unsigned short port)
{
    int ec = 0;
    sockaddr_in addr;

    ec = uv_ip4_addr(ip, port, &addr);
    if(ec){
        return ec;
    }

    ec = uv_tcp_bind((uv_tcp_t*)m_hot, (const sockaddr*)&addr, 0);
    if(ec){
        return ec;
    }

    auto on_new_conn = [](uv_stream_t * s, int status){
        if(status < 0){
            //TODO: error
            return;
        }

        Listener* lis = (Listener*)s->data;
        lis->m_mgr->do_accept((void*)s);
    };

    return uv_listen((uv_stream_t*)&m_hot, 1024, on_new_conn);
}
