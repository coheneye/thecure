#include <hub/signal.h>
#include <uv.h>


// ============================================= impl ===========================
class Signal::Impl {
public:
    Impl(Hub* h):m_h(h){
        m_hdl.data = (void*)this;
        uv_signal_init((uv_loop_t*)h->handle(), &m_hdl);
    }

    int start(int sig, function<void(int)> callback){
        m_cb_start = callback;
        return uv_signal_start(&m_hdl, cb_signal, sig);
    }

    int oneshot(int sig, function<void(int)> callback){
        m_cb_oneshot = callback;
        return uv_signal_start_oneshot(&m_hdl, cb_signal, sig);
    }

    int stop(){
        return uv_signal_stop(&m_hdl);
    }
protected:
    static void cb_signal(uv_signal_t* h, int signum){
        if(!h){
            return;
        }
        Impl* impl = (Impl*)h->data;
        //TODO: bug here.
        if(impl->m_cb_start){
            impl->m_cb_start(signum);
        }
        if(impl->m_cb_oneshot){
            impl->m_cb_oneshot(signum);
        }
    }
private:
    Hub* m_h;
    uv_signal_t m_hdl;
    function<void(int)> m_cb_start;
    function<void(int)> m_cb_oneshot;
};


// ============================================= Signal ==========================
Signal::Signal(Hub * h)
{
    m_impl = make_unique<Signal::Impl>(h);
}

Signal::~Signal() = default;


int Signal::start(int sig, function<void(int)> callback)
{
    m_impl->start(sig, callback);
}

int Signal::oneshot(int sig, function<void(int)> callback)
{
    return m_impl->oneshot(sig, callback);
}

int Signal::stop()
{
    return m_impl->stop();
}