#ifndef _THRCURE_TIMER_H_
#define _THRCURE_TIMER_H_

#include "hub.h"
#include <utils/luacxx.h>
#include <LuaBridge/LuaBridge.h>

using namespace luabridge;


class Timer final {
public:
    Timer(Hub* h);
    ~Timer();

public:
    /** start a timer countdown
     * 
     * @timeout: in milliseconds
     * @repeat: interval after the 1st timeout. in millisecond
     * @cb: callback when timeout
     * @data: user data, used as cb parameter when timeout.
     * @return some error code.
    */
    int start(uint64_t timeout, uint64_t repeat, std::function<void(void*)> cb, void* data=0);

    /* start() lua version */
    int start_lua(uint64_t timeout, uint64_t repeat, LuaRef f, LuaRef data);

    /** stop timer countdown
     * 
     */
    int stop();
    /** restart timer countdown
     * 
     */
    int again();

    bool is_active();

    void set_interval(uint64_t repeat);
    uint64_t get_interval();
private:
    class TimerImpl;
    std::unique_ptr<TimerImpl> m_impl;
};

#endif