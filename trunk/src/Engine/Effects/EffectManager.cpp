#include "EffectManager.h"
#include "Base/XML/tinyxml2/tinyxml2.h"
#include "Base/XML/XML.h"

CEffectManager::CEffectManager() {}
CEffectManager::~CEffectManager() {}

bool CEffectManager::Load(const std::string& aFilename)
{
    bool lOk = false;
    mFilename = aFilename;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

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

//TODO Reload