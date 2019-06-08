#include <hub/timer.h>
#include <utils/function.h>
#include <utils/lua_manager.h>
#include <uv.h>


class Timer::TimerImpl {
public:
    TimerImpl(Hub* h):m_lua_f(LL),
    m_lua_data(LL){
        m_timer.data = (void*)this;
        uv_timer_init((uv_loop_t*)h->handle(), &m_timer);
    }

    ~TimerImpl(){
        // stop
        // close?
    }

    int start(uint64_t timeout, uint64_t repeat, std::function<void(void*)> cb, void* data){
        this->m_data = data;
        m_cb = cb;
        return uv_timer_start(&m_timer, on_timer, timeout, repeat);
    }

    int start_lua(uint64_t timeout, uint64_t repeat, LuaRef f, LuaRef data){
        if(!f.isFunction()){
            //log
            return -1;
        }
        m_lua_f = f;
        m_lua_data = data;
        return uv_timer_start(&m_timer, on_lua_timer, timeout, repeat);
    }

    int stop(){
        return uv_timer_stop(&m_timer);
    }

    int again(){
        return uv_timer_again(&m_timer);
    }

    bool is_active(){
        return uv_is_active((const uv_handle_t*)&m_timer);
    }

    void set_interval(uint64_t repeat){
        return uv_timer_set_repeat(&m_timer, repeat);
    }

    uint64_t get_interval(){
        return uv_timer_get_repeat(&m_timer);
    }

private:
    static void on_timer(uv_timer_t* timer){
        Timer::TimerImpl* impl = (Timer::TimerImpl*)timer->data;
        if(impl->m_cb){
            impl->m_cb(impl->m_data);
        }        
    }
    static void on_lua_timer(uv_timer_t* timer){
        Timer::TimerImpl * impl = (Timer::TimerImpl*)timer->data;
        if(impl && impl->m_lua_f){
            impl->m_lua_f(impl->m_lua_data);
            //(*(impl->m_lua_f))(*(impl->m_lua_data));
        }
    }
protected:
    std::function<void(void*)> m_cb;
    void* m_data;

    LuaRef m_lua_f;
    LuaRef m_lua_data;
    uv_timer_t m_timer;
};

Timer::Timer(Hub* h)
{
    m_impl = make_unique<TimerImpl>(h);
}

Timer::~Timer() = default;

int Timer::start(uint64_t timeout, uint64_t repeat, std::function<void(void*data)> cb, void* data)
{
    return m_impl->start(timeout, repeat, cb, data);
}

int Timer::start_lua(uint64_t timeout, uint64_t repeat, LuaRef f, LuaRef data)
{
    return m_impl->start_lua(timeout, repeat, f, data);
}

int Timer::stop()
{
    return m_impl->stop();
}

int Timer::again()
{   
    return m_impl->again();
}

bool Timer::is_active()
{
    return m_impl->is_active();
}

void Timer::set_interval(uint64_t repeat)
{
    return m_impl->set_interval(repeat);
}

uint64_t Timer::get_interval()
{
    return m_impl->get_interval();
}
