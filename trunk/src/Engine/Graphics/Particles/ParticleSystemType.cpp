#include "Graphics\Particles\ParticleSystemType.h"
#include "Graphics\Materials\MaterialManager.h"
#include "Engine\engine.h"
#include "Utils\Defines.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Utils\EnumToString.h"
#include "Utils\Logger.h"
#include "XML\XML.h"


CParticleSystemType::CParticleSystemType()
{

}


CParticleSystemType::~CParticleSystemType()
{

}

CParticleSystemType::CParticleSystemType(const CXMLElement* aTreeNode)
    : m_NumFrames(aTreeNode->GetAttribute<int>("numFrames", 200))
    , m_TimePerFrame(aTreeNode->GetAttribute<float>("timePerFrame", 1.0f))
    , m_LoopFrames(aTreeNode->GetAttribute<bool>("loopFrames", true))
    , m_EmitAbsolute(aTreeNode->GetAttribute<bool>("emitAbsolute", true))
    , m_StartingDirectionAngle(aTreeNode->GetAttribute<float>("startingDirectionAngle", 1.0f))
    , m_StartingAccelerationAngle(aTreeNode->GetAttribute<float>("startingAccelerationAngle", 1.0f))
    , m_Size(aTreeNode->GetAttribute<Vect2f>("size", Vect2f(0.5f, 0.5f)))
    , m_EmitRate(aTreeNode->GetAttribute<Vect2f>("emitRate", Vect2f(0.5f, 0.5f)))
    , m_AwakeTime(aTreeNode->GetAttribute<Vect2f>("awakeTime", Vect2f(0.0f, 0.0f)))
    , m_SleepTime(aTreeNode->GetAttribute<Vect2f>("sleepTime", Vect2f(1.0f, 1.0f)))
    , m_Life(aTreeNode->GetAttribute<Vect2f>("life", Vect2f(0.3f, 0.3f)))
    , m_StartingAngle(aTreeNode->GetAttribute<Vect2f>("startingAngle", Vect2f(0.0f, 90.0f)))
    , m_StartingAngularSpeed(aTreeNode->GetAttribute<Vect2f>("startingAngularSpeed", Vect2f(1.0f, 1.0f)))
    , m_AngularAcceleration(aTreeNode->GetAttribute<Vect2f>("angularAcceleration", Vect2f(0.5f, 0.5f)))
    , m_StartingSpeed1(aTreeNode->GetAttribute<Vect3f>("startingSpeed1", Vect3f(0.3f, 0.3f, 0.3f)))
    , m_StartingSpeed2(aTreeNode->GetAttribute<Vect3f>("startingSpeed2", Vect3f(0.3f, 0.3f, 0.3f)))
    , m_StartingAcceleration1(aTreeNode->GetAttribute<Vect3f>("startingAcceleration1", Vect3f(0.0f, 0.3f, 0.0f)))
    , m_StartingAcceleration2(aTreeNode->GetAttribute<Vect3f>("startingAcceleration2", Vect3f(0.0f, 0.3f, 0.0f)))
    , m_Color1(aTreeNode->GetAttribute<CColor>("color1", CColor(1, 0, 0, 1)))
    , m_Color2(aTreeNode->GetAttribute<CColor>("color2", CColor(0, 1, 0, 1)))
{
    CMaterialManager& lMaterialManager = CEngine::GetInstance().GetMaterialManager();
    m_pmaterial = lMaterialManager(aTreeNode->GetAttribute<std::string>("material", ""));
    CName::SetName(aTreeNode->GetAttribute<std::string>("name", ""));

    const tinyxml2::XMLElement* aElement = aTreeNode->FirstChildElement();

    while (aElement != NULL)
    {
        if (strcmp(aElement->Name(),"control_point_size") == 0)
        {
            ControlPointSize lControlSize;
            lControlSize.m_Size = aElement->GetAttribute<Vect2f>("size", Vect2f(0.5, 0.5));
            lControlSize.m_Time = aElement->GetAttribute<Vect2f>("time", Vect2f(0.3, 0.3));
            m_ControlPointSizes.push_back(lControlSize);
        }
        if (strcmp(aElement->Name(), "control_point_color") == 0)
        {
            ControlPointColor lControlColor;
            lControlColor.m_Color1 = aElement->GetAttribute<CColor>("color1", CColor(1, 0, 0, 1));
            lControlColor.m_Color2 = aElement->GetAttribute<CColor>("color2", CColor(0, 0, 1, 1));
            lControlColor.m_Time = aElement->GetAttribute<Vect2f>("time", Vect2f(0.3, 0.3));
            m_ControlPointColors.push_back(lControlColor);
        }
        aElement = aElement->NextSiblingElement();
    }
}