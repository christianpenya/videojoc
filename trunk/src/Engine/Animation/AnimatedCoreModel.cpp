#include "XML\tinyxml2\tinyxml2.h"
#include "AnimatedCoreModel.h"
#include "XML\XML.h"
#include "coremodel.h"
#include "Materials/Material.h"


CAnimatedCoreModel::CAnimatedCoreModel(const std::string& aName)
    : CName(aName), m_BSRadius(0)
{
    m_Path = "";
    m_CalCoreModel = new CalCoreModel(aName);
}

CAnimatedCoreModel::~CAnimatedCoreModel() {}


void CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;

    bool lOk = false;
    std::string lFilename = "actor.xml";

    CXMLDocument document;

    EXMLParseError error = document.LoadFile((m_Path + lFilename).c_str());
    if (base::xml::SucceedLoad(error))
    {
        CXMLElement *lAnimatedModels = document.FirstChildElement("actor");
        if (lAnimatedModels)
        {
            for (tinyxml2::XMLElement *iAnimatedModel = lAnimatedModels->FirstChildElement(); iAnimatedModel != nullptr; iAnimatedModel = iAnimatedModel->NextSiblingElement())
            {
                if (strcmp(iAnimatedModel->Name(), "mesh") == 0)
                {
                    // std::string lName = iAnimatedModel->GetAttribute<std::string>("name", "");
                    LoadMesh(m_Path + iAnimatedModel->GetAttribute<std::string>("filename", ""));
                }
                else if (strcmp(iAnimatedModel->Name(), "skeleton") == 0)
                {
                    //std::string lName = iAnimatedModel->GetAttribute<std::string>("name", "");
                    LoadSkeleton(m_Path + iAnimatedModel->GetAttribute<std::string>("filename", ""));
                }
                else if (strcmp(iAnimatedModel->Name(), "material") == 0)
                {
                    //std::string lName = iAnimatedModel->GetAttribute<std::string>("name", "");
                    CMaterial * l_Material = new CMaterial(iAnimatedModel);
                    m_Materials.push_back(l_Material);
                }
                else if (strcmp(iAnimatedModel->Name(), "animation") == 0)
                {
                    std::string lName = iAnimatedModel->GetAttribute<std::string>("name", "");
                    LoadAnimation(lName, m_Path + iAnimatedModel->GetAttribute<std::string>("filename", ""));

                }

            }
            lOk = true;


        }
    }

    return;


}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    if (m_CalCoreModel->loadCoreMesh(Filename) == -1)
        return false;
    else
        return true;

}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{

    return m_CalCoreModel->loadCoreSkeleton(Filename);

}


bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{

    if (m_CalCoreModel->loadCoreAnimation(Filename) == -1)
        return false;
    else
        return true;

}

CalCoreModel * CAnimatedCoreModel::GetCoreModel()
{
    return m_CalCoreModel;
}

void CAnimatedCoreModel::DrawImgui()
{

}
