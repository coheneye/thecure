#ifndef _THECURE_TIME_H_
#define _THECURE_TIME_H_

#include <string>
#include <cstdint>
#include <chrono>

using namespace std;

uint64_t now_in_second();

uint64_t now_in_millsecond();

std::string today_in_str(const char* sep="-");

std::string now_in_str(const char* sep="-");


class TimeCounter {
public:
    TimeCounter();
    virtual ~TimeCounter();

    void reset();

    double elapse_secs() const;
    uint64_t elapse_mills() const;
private:
    chrono::time_point<chrono::high_resolution_clock> m_start;
};

#endif
