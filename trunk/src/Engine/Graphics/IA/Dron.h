#pragma once
#ifndef _DRON_20170809
#define _DRON_20170809

#include "Graphics/Mesh/Mesh.h"
#include "Input/CharacterController.h"
#include "Graphics\IA\enemy.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>


class CDron : public CEnemy
{

public:
    CDron(CXMLElement* aTreeNode);
    virtual ~CDron();

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
    Vect3f height = Vect3f(0.f, 1.5f, .0f);

    void patrol() override;
    void chase() override;
    CCharacterController m_DronController;

    void GotoNextPoint();
    void GotoNextPointChase();
    void ComputePath();
    void Move(Vect3f movement, Vect3f destination, float speed);

};

#endif //_DRON_20170809