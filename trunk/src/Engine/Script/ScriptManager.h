#pragma once
#ifndef _SCRIPTMANAGER_20170222_HH_
#define _SCRIPTMANAGER_20170222_HH_

#include "Utils\TemplatedMapVector.h"
#include "Script.h"

class CScriptManager
{
public:
    CScriptManager();
    virtual ~CScriptManager();
    CScript* GetScript(const std::string& aFilename);
    bool Load(const std::string& aFilename);
    bool Reload();
private:
    base::utils::CTemplatedMapVector<CScript> mScripts;

};

#endif //_SCRIPTMANAGER_20170222_HH_