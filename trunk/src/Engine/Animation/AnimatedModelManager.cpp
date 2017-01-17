#include "AnimatedModelManager.h"
#include "Base\XML\tinyxml2\tinyxml2.h"
#include "Base/XML/XML.h"
#include "Engine\Animation\AnimatedCoreModel.h"

CAnimatedModelManager::CAnimatedModelManager() {}
CAnimatedModelManager::~CAnimatedModelManager() {}

bool CAnimatedModelManager::Load(const std::string &Filename)
{
    bool lOk = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lAnimatedModels = document.FirstChildElement("animated_models");

        if (lAnimatedModels)
        {
            for (tinyxml2::XMLElement *iAnimatedModel = lAnimatedModels->FirstChildElement(); iAnimatedModel != nullptr; iAnimatedModel = iAnimatedModel->NextSiblingElement())
            {
                if (strcmp(iAnimatedModel->Name(), "scene") == 0)
                {
                    // #JORDI que fichero se carga aqui?
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
