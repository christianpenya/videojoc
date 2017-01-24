#include "AnimatedModelManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "XML\XML.h"
#include "AnimatedCoreModel.h"

CAnimatedModelManager::CAnimatedModelManager()
{
    CAnimatedModelManager::m_Filename = "";
}
CAnimatedModelManager::~CAnimatedModelManager() {}

bool CAnimatedModelManager::Load(const std::string &Filename)
{
    bool lOk = false;
    CAnimatedModelManager::m_Filename = Filename;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lAnimatedModels = document.FirstChildElement("animated_models");

        if (lAnimatedModels)
        {
            for (tinyxml2::XMLElement *iAnimatedModel = lAnimatedModels->FirstChildElement(); iAnimatedModel != nullptr; iAnimatedModel = iAnimatedModel->NextSiblingElement())
            {
                if (strcmp(iAnimatedModel->Name(), "animated_model") == 0)
                {

                    std::string lName = iAnimatedModel->GetAttribute<std::string>("name", "");
                    CAnimatedCoreModel* lAnimatedCoreModel = new CAnimatedCoreModel(lName);
                    lAnimatedCoreModel->Load(iAnimatedModel->GetAttribute<std::string>("path", ""));

                    Add(lAnimatedCoreModel->GetName(), lAnimatedCoreModel);
                }
            }
            lOk = true;
        }
    }

    return lOk;
}
