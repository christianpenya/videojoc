#pragma once

#ifndef _SCRIPT_20170222_HH_
#define _SCRIPT_20170222_HH_

#include "Utils\Name.h"
#include "lua_utils.h"

class CScript: public CName
{
public:
    CScript();
    virtual ~CScript();
    void Release();
    bool Load(const std::string& aFilename);
    bool operator()(const std::string& aCode);
    bool Reload();
    lua_State* GetState();

private:
    lua_State* mLS;
    std::string mFilename;
    void Init();
};

#endif //_SCRIPT_20170222_HH_
