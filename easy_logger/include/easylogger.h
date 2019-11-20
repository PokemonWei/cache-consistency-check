#ifndef EASY_LOGGER_HEADER
#define EASY_LOGGER_HEADER

#include<ostream>
#include<string>
#include<sstream>

#include "spdlog/async.h" 
#include "spdlog/sinks/basic_file_sink.h"


class EasyLogger
{
    public:

    enum LOG_LEVEL
    {
        debug,
        info,
        warn,
        error,
    };

    virtual ~EasyLogger();
    
    template <typename T>
    EasyLogger &operator<<(const T &rhs) {
        m_oss << rhs;
        return (*this);
    }

    explicit EasyLogger(std::string log_name,std::string log_path,LOG_LEVEL level)
    {
        async_log = spdlog::basic_logger_mt<spdlog::async_factory>(log_name, log_path);
        m_level = level;
    }
    
    
    explicit EasyLogger(std::string file_name,int line_num,std::string log_path,LOG_LEVEL level)
    {
        std::string log_name = "";
        log_name += file_name;
        log_name += " : ";
        log_name += std::to_string(line_num);
        async_log = spdlog::basic_logger_mt<spdlog::async_factory>(log_name, log_path);
        m_level = level;
    }
    

    private:
    LOG_LEVEL m_level;
    std::ostringstream m_oss;
    std::shared_ptr<spdlog::logger> async_log;
};

#define EasyLoggerWithTrace(p,l)             \
        EasyLogger(__FILE__, __LINE__,p,l)

EasyLogger::~EasyLogger()
{
    auto s = m_oss.str();
    switch (m_level)
    {
    case LOG_LEVEL::info:
        async_log->info(s);
        break;
    case LOG_LEVEL::debug:
        async_log->debug(s);
    case LOG_LEVEL::warn:
        async_log->warn(s);
        break;
    case LOG_LEVEL::error:
        async_log->error(s);
        break;
    default:
        break;
    }
}


#endif // !EASY_LOGGER_HEADER