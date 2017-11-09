#pragma once
#ifndef _GUARD_20170902
#define _GUARD_20170902

#include "Graphics/Mesh/Mesh.h"
#include "Input/CharacterController.h"
#include "Graphics\IA\EnemyAnimated.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>


class CGuard : public CEnemyAnimated
{

public:
    CGuard(CXMLElement* aTreeNode);
    virtual ~CGuard();

    GET_SET(float, DetectAngle);
    GET_SET(float, speedPatroling);
    GET_SET(float, speedChasing);
    GET_SET(float, investigatingTolerance);

    void DrawImgui();
    float m_DetectAngle;
    std::string m_soundDetected;
    float m_speedPatroling;
    float m_speedChasing;
    float m_investigatingTolerance;
    Vect3f m_lastPositionView;

    void dieEnemy(Vect3f pos);
    void patrol();
    void chase();

    void GotoNextPoint();
    void GotoNextPointChase();
    void ComputePath();
    void Move(Vect3f destination, float speed);
    void Sleep();
    bool m_sleep;
    float m_timeSleep;
};

#endif //_GUARD_20170902