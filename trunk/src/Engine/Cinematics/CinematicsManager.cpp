#include "CinematicsManager.h"




CCinematicManager::CCinematicManager()
{
    m_Filename = "";


}

CCinematicManager::~CCinematicManager()
{


}

bool CCinematicManager::Load(const std::string &Filename)
{
    m_Filename = Filename;
    return Load();

}

bool CCinematicManager::Load()
{

    bool lOk = false;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());
    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lCinematics = document.FirstChildElement("cinematics");
        //CXMLElement * lCinematics = document.FirstChildElement("cinematic");
        if (lCinematics)
        {
            //for (tinyxml2::XMLElement *iCinematic = lCinematics->ToElement(); iCinematic != nullptr; iCinematic = iCinematic->NextSiblingElement())
            for (tinyxml2::XMLElement *iCinematic = lCinematics->FirstChildElement(); iCinematic != nullptr; iCinematic = iCinematic->NextSiblingElement())
            {
                CCinematic* lCinematic = new CCinematic();
                lOk = lOk & lCinematic->Load(iCinematic);
                Add(lCinematic->GetName(), lCinematic);

            }

        }


    }
    return lOk;
}

bool CCinematicManager::Reload()
{
    Clear();
    return Load();

}

bool CCinematicManager::Update(float elapsedTime)
{
    bool lOk = true;
    for (TMapResources::iterator iCinematicMapEntry = m_ResourcesMap.begin(); iCinematicMapEntry != m_ResourcesMap.end(); ++iCinematicMapEntry)
    {
        CCinematic* lCinemat = iCinematicMapEntry->second.m_Value;

        if (lCinemat->IsActive())
        {
            lCinemat->Update(elapsedTime);
        }
    }

    return lOk;
}

bool CCinematicManager::Play(const std::string &Name)
{

    m_ResourcesMap.find(Name)->second.m_Value->Play();
    return true;

}