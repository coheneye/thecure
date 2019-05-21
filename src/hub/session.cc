#include <assert.h>
#include <memory>

#include "session.h"
#include "manager.h"


uint64_t Session::ms_stat_received = 0;
uint64_t Session::ms_stat_sent = 0;


Session::Session(Server* s):m_buf_tracer(0), 
    m_read_buf(DEF_RECEIVE_BUF_SIZE),
    m_send_buf(DEF_SEND_BUF_SIZE)
{
    uv_tcp_init(s->handle(), &m_hot);
    m_hot.data = (void*)this;

    m_disp = new EchoDispatcher();
}


Session::~Session()
{
    this->close();
}


int Session::accept(uv_stream_t* s)
{
    int ec = uv_accept(s, (uv_stream_t*)&this->m_hot);
    if(ec){
        this->close();
    }else{
        Manager::inst()->add(this);
    }
    
    return ec;
}


int Session::connect(const struct sockaddr* dest, int* dest_len)
{

}


int Session::start_read()
{
    auto on_alloc = [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf){
        Session* ses = (Session*)h->data;

        ses->m_read_buf.resize(suggested_size);
        buf->base = ses->m_read_buf.buf();
        buf->len = ses->m_read_buf.size();
    };

    auto on_read = [](uv_stream_t* h, ssize_t read, const uv_buf_t* buf){
        Session* ses = (Session*)h->data;
        if(read < 0){
            //@log
            ses->close();
        }else if(read > 0){
            //@where to process data
        }
    };
    return uv_read_start((uv_stream_t*)&m_hot, on_alloc, on_read);
}


int Session::send(const char* buf, int size)
{
    m_writer.b = uv_buf_init((char*)buf, size);

    auto on_sent = [](uv_write_t* h, int status){
        if(status){
            //log
            return;
        }
    };
    return uv_write((uv_write_t*)&m_writer, (uv_stream_t*)&m_hot, &m_writer.b, 1, on_sent);
}


int Session::close()
{
    auto on_closed = [](uv_handle_t* h){
        Session* ses = (Session*)h->data;
        h->data = nullptr;
        Manager::inst()->remove(ses);
    };

    uv_close((uv_handle_t*)&m_hot, on_closed);
    return 0;
}


int Session::id()
{
    return *(int*)&m_hot;
}
