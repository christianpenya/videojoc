#pragma once
#ifndef _ENEMY_20170820_H
#define _ENEMY_20170820_H

#include <memory>
#include "Utils\TemplatedMap.h"
#include <iostream>

#include "Utils/TemplatedMapVector.h"
#include "Math/Color.h"
#include "Utils/StringUtils.h"
#include "Utils/EnumToString.h"
#include "Graphics/Scenes/Layer.h"
#include "Physx/PhysxManager.h"
#include "Events/LevelController.h"


class CEnemy
{
public:
    enum EEnemyType
    {
        eDron = 0,
        eGuard = 1,
        eLaser = 2
    };

    CEnemy(CXMLElement* aElement);
    virtual ~CEnemy();
    CPhysXManager &m_PhysXManager;
    float m_DeadDistance;

    GET_SET(EEnemyType, EnemyType);
    GET_SET(float, DeadDistance);

    EEnemyType m_EnemyType;
    virtual void DrawImgui();
    double distanceBetweenTwoPoints(double x, double y, double a, double b);
    float getAngle(float x1, float z1, float x2, float z2);
    float getAngle2(float x1, float y1, float z1, float x2, float y2, float z2);
    float GetRandomValue(float min, float max);
    float mDeltaT = 0.0f;
};


Begin_Enum_String(CEnemy::EEnemyType)
{
    Enum_String_Id(CEnemy::eDron, "dron");
    Enum_String_Id(CEnemy::eGuard, "guard");
    Enum_String_Id(CEnemy::eLaser, "laser");
}
End_Enum_String;

#endif