#include <utils/time.h>
#include <iomanip>
#include <utils/function.h>


uint64_t now_in_second()
{
    return chrono::system_clock::to_time_t(chrono::system_clock::now());
}

uint64_t now_in_millsecond()
{
    auto now = chrono::system_clock::now();
    auto now_in_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());
    return now_in_ms.count();
}

std::string today_in_str(const char* sep)
{
    auto now = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *localtime(&now);
    return sf("%04d%s%02d%s%02d", tm.tm_year + 1900, sep, tm.tm_mon + 1, sep, tm.tm_mday);
}

std::string now_in_str(const char* sep)
{
    auto now = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *localtime(&now);
    return sf("%04d%s%02d%s%02d %02d:%02d:%02d", tm.tm_year + 1900, sep, tm.tm_mon + 1, sep, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec);
}


TimeCounter::TimeCounter():m_start(chrono::high_resolution_clock::now())
{
    
}

TimeCounter::~TimeCounter()=default;

void TimeCounter::reset()
{
    m_start = chrono::high_resolution_clock::now();
}

double TimeCounter::elapse_secs() const
{
    return chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - m_start).count();
}

uint64_t TimeCounter::elapse_mills() const
{
    return chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - m_start).count();
}
