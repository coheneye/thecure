#include <utils/lua_manager.h>
#include <utils/logger.h>


bool LuaManager::init_lua()
{
    if(!m_state){
        m_state = luaL_newstate();
        luaL_openlibs(m_state);
    }
    return true;
}

lua_State* LuaManager::get_state()
{
    return m_state;
}

int LuaManager::do_file(const char* filename)
{
    int ec = luaL_dofile(m_state, filename);
    if(ec){
        //log?
        gl_error(lua_tostring(m_state, -1));
    }
    return ec;
}
