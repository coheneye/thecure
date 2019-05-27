#ifndef _THECURE_LOGGER_H_
#define _THECURE_LOGGER_H_


/**
 * logger interface MUST export to lua
 * log to both console and files(daily) asynchronously by default.
 * when to file, log level is 'warn'
 * when to console, specified by user.
 */

extern "C" {
    #include <lua.h>
}
#include <memory>
#include <singleton.h>

// log level
// copy from spdlog
#define LOGGER_LEVEL_TRACE 0
#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 3
#define LOGGER_LEVEL_ERROR 4
#define LOGGER_LEVEL_CRITICAL 5
#define LOGGER_LEVEL_OFF 6


class logger final : public Singleton<logger> {
    class impl;
public:
    logger(){}
    ~logger(){}

    bool init(int console_level);

    void trc(const char* s);
    void dbg(const char* s);
    void inf(const char* s);
    void wrn(const char* s);
    void err(const char* s);
    void cri(const char* s);
private:
    std::shared_ptr<impl> m_logger;
};

typedef logger Logger;

#define LOG (Logger::get_inst())

#endif
