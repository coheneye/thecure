#include <utils/logger.h>
#include <utils/function.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>


class logger::impl {
public:
    impl() = default;
    ~impl() = default;

    bool init(const char* name, int console_level)
    {
        const char* default_name = "default";
        if(!name){
            name = default_name;
        }
        spdlog::set_error_handler([](const std::string &msg) { 
            spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); 
        });

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(static_cast<spdlog::level::level_enum>(console_level));
        // use default format for now.
        //console_sink->set_formatter("");
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(name, 0, 0, true);
        file_sink->set_level(spdlog::level::info);
    
        spdlog::init_thread_pool(8192, 1);  //queue 8k items and 1 backing thread

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        m_async_logger = std::make_shared<spdlog::async_logger>(name, 
            sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        // NEVER delete this line below.
        m_async_logger->set_level(spdlog::level::info);

        spdlog::register_logger(m_async_logger);

        return true;
    }
    
    std::shared_ptr<spdlog::logger> m_async_logger;
};

//============================================= logger ====================

logger::logger()
{
    m_logger = make_unique<logger::impl>();
}

logger::~logger() = default;

bool logger::init(const char* name, int console_level)
{   
    return m_logger->init(name, console_level);
}

void logger::cri(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->critical(content);
}


void logger::dbg(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->debug(content);
}


void logger::err(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->error(content);
}


void logger::inf(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->info(content);
}


void logger::trc(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->trace(content);
}


void logger::wrn(const char* s, const char* file, int line, const char* func)
{
    string content = string_format(g_format, s, get_file_name(file), line, func);
    m_logger->m_async_logger->warn(content);
}