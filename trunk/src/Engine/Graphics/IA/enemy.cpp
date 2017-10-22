#include "enemy.h"
#include "Engine/engine.h"
#include "Utils/Logger.h"
#include "Utils\EnumToString.h"
#include "ImGUI\imgui.h"
#include "XML\XML.h"
#include "Graphics/Mesh/MeshManager.h"

CEnemy::~CEnemy()
{
}

CEnemy::CEnemy(CXMLElement* aElement)
    : m_PhysXManager(CEngine::GetInstance().GetPhysXManager())
    , resultado(new CPhysXManager::RaycastData())
    , m_DeadDistance(aElement->GetAttribute<float>("deadDistance", 1.8f))
{
    EnumString<EEnemyType>::ToEnum(m_EnemyType, aElement->GetAttribute<std::string>("type", "dron"));
}

double CEnemy::distanceBetweenTwoPoints(double x, double y, double a, double b)
{
    return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

void CEnemy::DrawImgui() {}


float CEnemy::getAngle(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float theta = atan2(z1 - z2, x1 - x2);
    return -theta * 180 / 3.1415926;
}
float CEnemy::getAngle2(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
    float dist2 = sqrt(pow(x1 - x2, 2) + pow(z1 - z2, 2));
    return acos(dist2 / dist) * 180 / 3.1415926;
}