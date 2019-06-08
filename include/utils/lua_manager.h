#ifndef _THECURE_LUA_STATE_H_
#define _THECURE_LUA_STATE_H_

#include "luacxx.h"
#include "singleton.h"

class LuaManager : public Singleton<LuaManager>
{
public:
    bool init_lua();
    
    lua_State* get_state();

    int do_file(const char* filename);

protected:
    lua_State* m_state;
};

#define LL LuaManager::get_inst()->get_state()

#endif
