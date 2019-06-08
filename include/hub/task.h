#ifndef __THECURE_TASK_H_
#define __THECURE_TASK_H_


#include "hub.h"
#include <utils/luacxx.h>
#include <LuaBridge/LuaBridge.h>

using namespace luabridge;


class Task final {
public:
    Task(Hub* h);
    ~Task();

    int run(std::function<void(Task*, void*)> worker,          // 线程函数, 参数 void* 就是传的 worker_param
        void* worker_param, 
        std::function<void(Task*, void*)> worker_progress,     // 如果在 worker 中调用本函数 send，就会执行本回调, void* 为 send 的值
        std::function<void(int)>worker_done);           // worker 结束后调用 ,int 表示结束状态

    int run_lua(LuaRef worker, LuaRef worker_param, LuaRef worker_progress, LuaRef worker_done);

    // 取消 run, 只是提供一种手段，需要 worker 实现的配合
    int cancel();

    int send(void* data);

    int send_lua(LuaRef data);
private:
    class TaskImpl;
    std::unique_ptr<TaskImpl> m_impl;
};


#endif