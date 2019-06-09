#include <memory>
#include <string>
#include <string.h> // strlen
#include <functional>


using namespace std;

#if defined(_WIN32)
    #define PATH_SEP '\\'
    #define ENV_SEP ';'
#else
    #define PATH_SEP '/'
    #define ENV_SEP ':'
#endif

// golang-like defer
using defer = std::shared_ptr<void>;


template<typename T, typename ...Args>
unique_ptr<T> make_unique(Args&& ...args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}


template<typename ... Args>
std::string string_format( const char* format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format, args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format, args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

#define sf string_format


/** 从路径中获取文件名(utf-8 not supported now)
 * eg. 
 * /usr/local/lib/test.a    returns  test.a
 */
const char* get_file_name(const char* fullname);


const char* version();


const char* platform();


uint64_t get_total_mem();


uint64_t get_constrained_mem();


double get_uptime();


std::string get_exe_path();
