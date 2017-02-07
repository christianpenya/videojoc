#include "EffectManager.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"

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
    //#TODO o no TODO
    return true;
}

bool CEffectManager::Refresh()
{
    for (std::vector<CEffect*>::iterator iEffect = m_ResourcesVector.begin(); iEffect != m_ResourcesVector.end(); ++iEffect)
    {
        (*iEffect)->Refresh();
    }

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
