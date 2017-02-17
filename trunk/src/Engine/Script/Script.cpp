#include "Script.h"
#include "Engine/Engine.h"

CScript::CScript()
{
    Init();
}

CScript::~CScript()
{
}

void CScript::Release()
{
    if (mLS)
        lua_close(mLS);
}

bool CScript::Load(const std::string& aFilename)
{
    mFilename = aFilename;
    int status = luaL_loadfile(mLS, mFilename.c_str());
    auto const lua_ok = LUA_OK;
    if (status != lua_ok)
    {
        if (status == LUA_ERRSYNTAX)
            printf("[LUA]Sintax Error: %s", lua_tostring(mLS, -1));

        else if (status == LUA_ERRFILE)
            printf("[LUA]File Error: %s", lua_tostring(mLS, -1));

        else if (status == LUA_ERRMEM)
            printf("[LUA]Memory Error: %s", lua_tostring(mLS, -1));

        return false;
    }

    status = lua_pcall(mLS, 0, LUA_MULTRET, 0);
    if (status != lua_ok)
    {
        std::string err = lua_tostring(mLS, -1);
        printf("[LUA]Error executing code %s", err.c_str());
    }

    return true;
}

bool CScript::operator()(const std::string& aCode)
{
    luaL_dostring(mLS, aCode.c_str());
    return true;
}

bool CScript::Reload()
{
    Release();
    Init();
    return Load(mFilename);

}

lua_State* CScript::GetState()
{
    return mLS;
}

void CScript::Init()
{
    mLS = luaL_newstate();
    luaL_openlibs(mLS);
    luabind::open(mLS);
    lua_atpanic(mLS, lua::atpanic);
    lua_register(mLS, "_ALERT", lua::atpanic);

    //REGISTER_LUA_FUNCTION(mLS, "print_int", &PrintInt);

}