#include <hub/task.h>
#include <utils/function.h>
#include <utils/lua_manager.h>
#include <uv.h>


class Task::TaskImpl {
public:
    TaskImpl(Task* t, Hub* h):m_task(t), m_hub(h),
        m_worker_lua(LL), m_param_lua(LL), m_sent_lua(LL), m_progres_lua(LL), m_done_lua(LL){
        uv_async_init((uv_loop_t*)h->handle(), &m_async, Task::TaskImpl::on_progress);
        m_async.data = (void*)this;
        m_hdl.data = (void*)this;
    }

    ~TaskImpl(){
        uv_close((uv_handle_t*)&m_async, 0);
        uv_close((uv_handle_t*)&m_hdl, 0);
    }

    int run(std::function<void(Task*, void*)> worker,          // 线程函数, 参数 void* 就是传的 worker_param
        void* worker_param, 
        std::function<void(Task*, void*)> worker_progress,     // 如果在 worker 中调用本函数 send，就会执行本回调, void* 为 send 的值
        std::function<void(int)>worker_done){           // worker 结束后调用 ,int 表示结束状态
        m_worker = worker;
        m_param = worker_param;
        m_progress = worker_progress;
        m_done = worker_done;

        return uv_queue_work((uv_loop_t*)m_hub->handle(), &m_hdl, Task::TaskImpl::worker, Task::TaskImpl::on_done);
    }

    int run_lua(LuaRef worker, LuaRef worker_param, LuaRef worker_progress, LuaRef worker_done){
        m_worker_lua = worker;
        m_param_lua = worker_param;
        m_progres_lua = worker_progress;
        m_done_lua = worker_done;

        return uv_queue_work((uv_loop_t*)m_hub->handle(), &m_hdl, Task::TaskImpl::worker_lua, TaskImpl::on_done_lua);
    }
    // 取消 run, 只是提供一种手段，需要 worker 实现的配合
    int cancel(){
        return uv_cancel((uv_req_t*)&m_hdl);
    }

    int send(void* data){
        m_sent = data;
        return uv_async_send(&m_async);
    }

    int send_lua(LuaRef data){
        m_sent_lua = data;
        return uv_async_send(&m_async);
    }
protected:
    static void worker(uv_work_t* req){
        Task::TaskImpl* impl = (Task::TaskImpl*)req->data;
        impl->m_worker(impl->m_task, impl->m_param);
    }
    static void on_progress(uv_async_t* h){
        Task::TaskImpl* impl = (Task::TaskImpl*)h->data;
        if(impl->m_progress){
            impl->m_progress(impl->m_task, impl->m_sent);
        }
        if(impl->m_progres_lua.isFunction()){
            impl->m_progres_lua(impl->m_sent_lua);
        }
    }
    static void on_done(uv_work_t* req, int status){
        Task::TaskImpl* impl = (Task::TaskImpl*)req->data;
        impl->m_done(status);
    }

    static void worker_lua(uv_work_t * req){
        TaskImpl * impl = (TaskImpl*)req->data;
        if(impl->m_worker_lua.isFunction()){
            impl->m_worker_lua(impl->m_param_lua);
        }
    }

    static void on_done_lua(uv_work_t* req, int status){
        TaskImpl* impl = (TaskImpl*)req->data;
        if(impl->m_done_lua.isFunction()){
            impl->m_done_lua(status);
        }
    }
    uv_work_t m_hdl;
    uv_async_t m_async;

    Task* m_task;
    void* m_param;
    void* m_sent;

    LuaRef m_param_lua;
    LuaRef m_sent_lua;

    LuaRef m_worker_lua;
    LuaRef m_progres_lua;
    LuaRef m_done_lua;
    std::function<void(Task*, void*)> m_worker;
    std::function<void(Task*, void*)> m_progress;
    std::function<void(int)> m_done;
    Hub* m_hub;
};


//================================================== Task Implementation ========================================

Task::Task(Hub* h)
{
    m_impl = make_unique<TaskImpl>(this, h); //new Task::TaskImpl(this, h);
}

Task::~Task() = default;

int Task::run(std::function<void(Task*, void*)> worker,          // 线程函数, 参数 void* 就是传的 worker_param
        void* worker_param, 
        std::function<void(Task*, void*)> worker_progress,     // 如果在 worker 中调用本函数 send，就会执行本回调, void* 为 send 的值
        std::function<void(int)>worker_done)           // worker 结束后调用 ,int 表示结束状态
{
    return m_impl->run(worker, worker_param, worker_progress, worker_done);
}

int run_lua(LuaRef worker, LuaRef worker_param, LuaRef worker_progress, LuaRef worker_done)
{

}
    // 取消 run, 只是提供一种手段，需要 worker 实现的配合
int Task::cancel()
{
    return m_impl->cancel();
}

int Task::send(void* data)
{
    return m_impl->send(data);
}

int Task::send_lua(LuaRef data)
{
    return m_impl->send_lua(data);
}

int Task::run_lua(LuaRef worker, LuaRef worker_param, LuaRef worker_progress, LuaRef worker_done)
{
    return m_impl->run_lua(worker, worker_param, worker_progress, worker_done);
}
