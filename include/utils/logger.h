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
#include "hub/hub.h"
#include "singleton.h"

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
public:
    logger();
    ~logger();

    bool init(const char* name, int console_level=LOGGER_LEVEL_DEBUG, int file_level=LOGGER_LEVEL_INFO);
    bool stop(); // make async logger quit.

    void trc(const char* s, const char* file, int line, const char* func);
    void dbg(const char* s, const char* file, int line, const char* func);
    void inf(const char* s, const char* file, int line, const char* func);
    void wrn(const char* s, const char* file, int line, const char* func);
    void err(const char* s, const char* file, int line, const char* func);
    void cri(const char* s, const char* file, int line, const char* func);

private:
    class impl;
    std::unique_ptr<impl> m_logger;
};

typedef logger Logger;

#define LOG (Logger::get_inst())

#define gl_trace(s) LOG->trc(s, __FILE__, __LINE__, __FUNCTION__)
#define gl_debug(s) LOG->dbg(s, __FILE__, __LINE__, __FUNCTION__)
#define gl_info(s)  LOG->inf(s, __FILE__, __LINE__, __FUNCTION__)
#define gl_warn(s)  LOG->wrn(s, __FILE__, __LINE__, __FUNCTION__)
#define gl_error(s) LOG->err(s, __FILE__, __LINE__, __FUNCTION__)
#define gl_crit(s)  LOG->cri(s, __FILE__, __LINE__, __FUNCTION__)

#endif
