extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <LuaBridge/LuaBridge.h>
#include <iostream>
#include <unistd.h>

//in lua
void 
jprint(const char* content){
    std::cout << content << std::endl;
}

// in lua
class 
Line{
public:
    Line(const std::string& data):m_data(data){}

    std::size_t length() const {
        return m_data.length();
    }

protected:
    std::string m_data;
};

// main
int
main(int argc, char** argv)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Line>("Line")
            .addConstructor<void(*)(const std::string&)>()
            .addFunction("get_length", &Line::length)
        .endClass()
        .addFunction("jprint", jprint);
    
    const char* lua_code = " \
        local line = Line('test') \n \
        print(line:get_length())\n \
    ";
    // luaL_loadfile(L, "main.lua");
    luaL_dostring(L, lua_code);

    // say this is an event loop
    // while(1){
    //     sleep(1);
    // }
    
    lua_close(L);

    return 0;
}
