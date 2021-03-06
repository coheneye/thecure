#include <assert.h>
#include <memory>
#include <uv.h>
#include <utils/function.h>
#include <hub/session.h>
#include <iostream>


typedef struct writer_t{
    uv_write_t w;
    uv_buf_t b;
}mv_write_t;


ISession::ISession(Hub *h, IDispatcher* disp, Manager* m):m_hub(h),m_disp(disp),m_mgr(m)
{
    m_hdl = malloc(sizeof(uv_tcp_t));
    
    uv_tcp_t* tmp = (uv_tcp_t*)m_hdl;
    tmp->data = (void*)this;
    uv_tcp_init((uv_loop_t*)m_hub->handle(), tmp);

    m_writer = malloc(sizeof(mv_write_t));
    uv_write_t* t = (uv_write_t*)m_writer;
    t->data = (void*)this;
}


ISession::~ISession()
{
    //this->inner_close();
    free(m_writer);
    free(m_hdl);
}

int ISession::connect(const char* ip, int port)
{
    auto on_conn_cb = [](uv_connect_t* conn, int status){
        ISession* ses = (ISession*)conn->data;
        assert(ses);
        
        defer _(nullptr, [conn](...){
            free(conn);
        });
        if(status){
            //TODO
            std::cout << "connect failed:" << uv_err_name(status) << std::endl;
            ses->m_mgr->do_connection_lost(ses);
            return;
        }
        ses->m_mgr->do_connected(ses);
    };
    uv_connect_t* conn = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    assert(conn);
    conn->data = (void*)this;

    struct sockaddr_in dest;
    int ec = uv_ip4_addr(ip, port, &dest);
    if(ec){
        std::cout<< "failed to connect to" << ip << std::endl;
        return ec;
    }

    ec = uv_tcp_connect(conn, (uv_tcp_t*)m_hdl, (const sockaddr*)&dest, on_conn_cb);
    return ec;
}

int ISession::start_read()
{
    auto on_alloc = [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf){
        ISession* ses = (ISession*)h->data;
        char* b;
        unsigned int l = suggested_size;
        ses->m_disp->lend(&b, &l);
        *buf = uv_buf_init(b, l);
    };

    auto on_read = [](uv_stream_t* h, ssize_t read, const uv_buf_t* buf){
        ISession* ses = (ISession*)h->data;
        if(read < 0){
            if (read == UV_EOF){
                std::cout<<"eof"<<std::endl;
                ses->close();
            }else{
                //@log
                std::cout<<"read error:"<<uv_err_name(read)<<std::endl;
                ses->m_mgr->do_read_error(ses, read);
            }
            
        }else if(read > 0){
            if(ses->m_disp){
                ses->m_disp->on_data_read(ses, buf->base, buf->len);
            }else{
                //@log
            }
            
        }
        if(ses->m_disp){
            ses->m_disp->remand(buf->base, buf->len);
        }
    };
    return uv_read_start((uv_stream_t*)m_hdl, on_alloc, on_read);
}


int ISession::send(const char* buf, unsigned int size)
{
    mv_write_t* w = (mv_write_t*)m_writer;

    w->b = uv_buf_init((char*)buf, size);

    auto on_sent = [](uv_write_t* h, int status){
        ISession* ses = (ISession*)h->data;
        if(status){
            //log
            ses->m_mgr->do_write_error(ses, status);
            return;
        }
    };
    return uv_write((uv_write_t*)m_writer, (uv_stream_t*)m_hdl, &w->b, 1, on_sent);
}


int ISession::inner_close()
{
    uv_close((uv_handle_t*)m_hdl, 0);
}


int ISession::close()
{
    auto on_closed = [](uv_handle_t* h){
        ISession* ses = (ISession*)h->data;
        ses->m_mgr->do_closed(ses);
    };

    uv_close((uv_handle_t*)m_hdl, on_closed);
    return 0;
}


int64_t ISession::id() const
{
    return *(int64_t*)m_hdl;
}

void* ISession::handle()
{
    return (void*)m_hdl;
}

void* ISession::get_tag()
{
    return m_tag;
}

void ISession::set_tag(void* tag)
{
    m_tag = tag;
}