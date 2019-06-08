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


/** 从路径中获取文件名(utf-8 not supported now)
 * eg. 
 * /usr/local/lib/test.a    returns  test.a
 */
inline const char* get_file_name(const char* fullname)
{
    for(int i = strlen(fullname); i >= 0; i--){
        if(fullname[i] == PATH_SEP){
            return fullname + i + 1;
        }
    }
    return fullname;
}

#define sf string_format
