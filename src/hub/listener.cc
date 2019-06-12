#include <assert.h>
#include <hub/listener.h>
#include <hub/session.h>
#include <hub/manager.h>
#include <uv.h>
#include <iostream>
#include <utils/logger.h>
#include <utils/function.h>


Listener::Listener(Hub* s, Manager* m):m_hub(s),m_mgr(m)
{
    m_hdl = malloc(sizeof(uv_tcp_t));
    uv_tcp_t* tmp = (uv_tcp_t*)m_hdl;
    tmp->data = (void*)this;

    int ec = uv_tcp_init((uv_loop_t*)s->handle(), tmp);
    if(0 != ec){
        gl_error(sf("uv_tcp_init() failed. ec=%d", ec).c_str());
    }
}


Listener::~Listener()
{
    auto on_closed = [](uv_handle_t* h){
        gl_trace("listener closed.");
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
        gl_error(sf("uv_ip4_addr() failed.[addr=%s#%d, ec=%d]", ip, (int)port, ec).c_str());
        return ec;
    }

    ec = uv_tcp_bind((uv_tcp_t*)m_hdl, (const sockaddr*)&addr, 0);
    if(ec){
        gl_error(sf("bind failed.[addr=%s#%d, ec=%d]", ip, (int)port, ec).c_str());
        return ec;
    }

    auto on_new_conn = [](uv_stream_t * s, int status){
        if(status < 0){
            gl_warn(sf("new connection comes with an error:%d", status).c_str());
            return;
        }

        Listener* lis = (Listener*)s->data;
        int ret = lis->m_mgr->do_accept((void*)s);
        if (ret){
            gl_warn(sf("do_accept() failed:%d", ret).c_str());
        }
    };

    return uv_listen((uv_stream_t*)m_hdl, 1024, on_new_conn);
}
