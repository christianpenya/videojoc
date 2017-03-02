#include "AnimatedModelManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "XML\XML.h"
#include "AnimatedCoreModel.h"
#include "cal3d.h"

CAnimatedModelManager::CAnimatedModelManager()
{
    CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS | LOADER_INVERT_V_COORD);
    CAnimatedModelManager::m_Filename = "";
}
CAnimatedModelManager::~CAnimatedModelManager()
{
    CTemplatedMap<CAnimatedCoreModel>::Destroy();
}

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
