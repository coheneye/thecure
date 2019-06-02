#include <hub/idle.h>
#include <uv.h>


class Idle::IdleImpl {
public:
    IdleImpl(Hub*h){
        m_idle.data = (void*)this;
        uv_idle_init((uv_loop_t*)h->handle(), &m_idle);
    }

    ~IdleImpl(){
        this->stop();
    }

    int start(std::function<void(void*)> cb, void* param){
        m_cb = cb;
        m_param = param;
        return uv_idle_start(&m_idle, on_idle);
    }

    int stop(){
        return uv_idle_stop(&m_idle);
    }
private:
    static void on_idle(uv_idle_t* h){
        Idle::IdleImpl* impl = (Idle::IdleImpl*)h->data;
        impl->m_cb(impl->m_param);
    }

    std::function<void(void*)> m_cb;
    void* m_param;
    uv_idle_t m_idle;
};


Idle::Idle(Hub* h)
{
   m_impl = new Idle::IdleImpl(h);
}


Idle::~Idle()
{
    delete m_impl;
}

int Idle::start(std::function<void(void*)> cb, void* param)
{
    return m_impl->start(cb, param);
}


int Idle::stop()
{
    return m_impl->stop();
}