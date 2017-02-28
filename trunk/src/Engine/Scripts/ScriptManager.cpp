#include "ScriptManager.h"

CScriptManager::CScriptManager()
{

}

CScriptManager::~CScriptManager()
{
    mScripts.Destroy();
}

CScript* CScriptManager::GetScript(const std::string& aFilename)
{
    if (!mScripts.Exist(aFilename))
    {
        Load(aFilename);
    }

    CScript* lScript = mScripts(aFilename);
    return lScript;
}

bool CScriptManager::Load(const std::string& aFilename)
{
    bool lOk = false;
    CScript * lScript = new CScript();

    if (lScript->Load(aFilename))
    {
        lOk = mScripts.Add(aFilename, lScript);
    }
    else
    {
        base::utils::CheckedDelete(lScript);
    }

    return lOk;

}
