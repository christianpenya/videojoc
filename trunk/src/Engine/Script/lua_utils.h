#ifndef _LUA_UTILS_HH_
#define _LUA_UTILS_HH_
#pragma once

#include <string>
#include <sstream>

// Lua Library includes
#include <lua/lua.hpp>

// LuaBind Library includes
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;

#define REGISTER_LUA_FUNCTION(LuaState, FunctionName,AddrFunction) {luabind::module(LuaState) [ luabind::def(FunctionName,AddrFunction) ];}

namespace lua
{
enum EngineLibrary
{
    Engine = 0,
    Base,
    Graphics,
    Render,
    Input,
    ThirdParty
};


enum class lib
{
    base = 0,
    package,
    coroutine,
    string,
    os,
    math,
    table,
    debug,
    bit32,
    io,
    count
};

inline int atpanic(lua_State* L)
{
    std::string err = lua_tostring(L, -1);
    printf("[LUA]: %s", err.c_str());
    return 0;
}

//template < typename T > void BindClass(lua_State *aLua);
//template < uint32 T > void BindLibrary(lua_State *aLua);

template<typename... Args>
void OpenLibraries(lua_State* L, Args&&... args)
{
    if (sizeof...(args) == 0)
    {
        luaL_openlibs(L);
        return;
    }

    lua::lib libraries[1 + sizeof...(args)] = { lua::lib::count, std::forward<Args>(args)... };

    for (auto&& library : libraries)
    {
        switch (library)
        {
        case lua::lib::base:
            luaL_requiref(L, "base", luaopen_base, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::package:
            luaL_requiref(L, "package", luaopen_package, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::coroutine:
            luaL_requiref(L, "coroutine", luaopen_coroutine, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::string:
            luaL_requiref(L, "string", luaopen_string, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::table:
            luaL_requiref(L, "table", luaopen_table, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::math:
            luaL_requiref(L, "math", luaopen_math, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::bit32:
            luaL_requiref(L, "bit32", luaopen_bit32, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::io:
            luaL_requiref(L, "io", luaopen_io, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::os:
            luaL_requiref(L, "os", luaopen_os, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::debug:
            luaL_requiref(L, "debug", luaopen_debug, 1);
            lua_pop(L, 1);
            break;
        case lua::lib::count:
            break;
        }
    }
}
}

#endif // _LUA_UTILS_HH_