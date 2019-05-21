#include "logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>


class logger::impl {
public:
    bool init(int console_level)
    {
        spdlog::set_error_handler([](const std::string &msg) { 
            spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); 
        });
    
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);//static_cast<spdlog::level::level_enum>(console_level));
        // use default format for now.
        //console_sink->set_formatter("");
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("log.log", 0, 0, true);
        file_sink->set_level(spdlog::level::info);
    
        spdlog::init_thread_pool(8192, 1);  //queue 8k items and 1 backing thread

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        m_async_logger = std::make_shared<spdlog::async_logger>("async", 
            sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        // NEVER delete this line below.
        m_async_logger->set_level(spdlog::level::info);

        spdlog::register_logger(m_async_logger);

        return true;
    }
    
    void log(int level, const char* s)
    {
        switch(level){
            case LOGGER_LEVEL_TRACE:
                return m_async_logger->trace(s);
            case LOGGER_LEVEL_DEBUG:
                return m_async_logger->debug(s);
            case LOGGER_LEVEL_INFO:
                return m_async_logger->info(s);
            case LOGGER_LEVEL_WARN:
                return m_async_logger->warn(s);
            case LOGGER_LEVEL_ERROR:
                return m_async_logger->error(s);
            case LOGGER_LEVEL_CRITICAL:
                return m_async_logger->critical(s);
            default:
                spdlog::error("invlid log level config");
                return spdlog::error(s);
        }
    }

private:
    std::shared_ptr<spdlog::logger> m_async_logger;
};


bool logger::init(int console_level)
{   
    m_logger = std::make_shared<logger::impl>();
    return m_logger->init(console_level);
}


void logger::cri(const char* s)
{
    m_logger->log(LOGGER_LEVEL_CRITICAL, s);
}


void logger::dbg(const char* s)
{
    m_logger->log(LOGGER_LEVEL_DEBUG, s);
}


void logger::err(const char* s)
{
    m_logger->log(LOGGER_LEVEL_ERROR, s);
}


void logger::inf(const char* s)
{
    m_logger->log(LOGGER_LEVEL_INFO, s);
}


void logger::trc(const char* s)
{
    m_logger->log(LOGGER_LEVEL_TRACE, s);
}


void logger::wrn(const char* s)
{
    m_logger->log(LOGGER_LEVEL_WARN, s);
}