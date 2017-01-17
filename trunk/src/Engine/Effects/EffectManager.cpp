#include "EffectManager.h"
#include "Base/XML/tinyxml2/tinyxml2.h"
#include "Base/XML/XML.h"

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
    Destroy();
}

bool CEffectManager::Load(const std::string& aFilename)
{
    mFilename = aFilename;
    return Load();
}

bool CEffectManager::Reload()
{
    Clear();
    return Load();
}

bool CEffectManager::Load()
{
    bool lOk = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((mFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lEffects = document.FirstChildElement("effects");

        if (lEffects)
        {
            for (tinyxml2::XMLElement *iEffect = lEffects->FirstChildElement(); iEffect != nullptr; iEffect = iEffect->NextSiblingElement())
            {
                if (strcmp(iEffect->Name(), "effect") == 0)
                {
                    CEffect* lEffect = new CEffect(iEffect);
                    Add(lEffect->GetName(), lEffect);
                }
            }
            lOk = true;
        }
    }

    return lOk;
}
