#include <utils/function.h>
#include <uv.h>


const char* get_file_name(const char* fullname)
{
    for(int i = strlen(fullname); i >= 0; i--){
        if(fullname[i] == PATH_SEP){
            return fullname + i + 1;
        }
    }
    return fullname;
}


const char* version()
{
    return uv_version_string();
}

const char* platform()
{
#if defined(_WIN32)
    return "windows";
#elif defined(__APPLE__)
    return "MAC OS"
#elif defined(__linux__)
    return "linux";
#endif
}

uint64_t get_total_mem()
{
    return uv_get_total_memory();
}

uint64_t get_free_mem()
{
    return uv_get_free_memory();
}