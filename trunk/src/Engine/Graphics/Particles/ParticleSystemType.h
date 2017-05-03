#pragma once
#ifndef _PARTICLESYSTEMTYPE_20170424
#define _PARTICLESYSTEMTYPE_20170424

#include "Graphics\Materials\Material.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>


class CParticleSystemType : public CName
{
private:
    struct ControlPointColor
    {
        Vect2f m_Time;
        CColor m_Color1, m_Color2;
    };

    struct ControlPointSize
    {
        Vect2f m_Time;
        Vect2f m_Size;
    };


public:
    CParticleSystemType();
    virtual ~CParticleSystemType();
    CParticleSystemType(const CXMLElement* aTreeNode);

    GET_SET(int, NumFrames);
    GET_SET(float, TimePerFrame);
    GET_SET(bool, LoopFrames);
    GET_SET(bool, EmitAbsolute);
    GET_SET(float, StartingDirectionAngle);
    GET_SET(float, StartingAccelerationAngle);
    GET_SET_REF(Vect2f, Size);
    GET_SET_REF(Vect2f, EmitRate);
    GET_SET_REF(Vect2f, AwakeTime);
    GET_SET_REF(Vect2f, SleepTime);
    GET_SET_REF(Vect2f, Life);
    GET_SET_REF(Vect2f, StartingAngle);
    GET_SET_REF(Vect2f, StartingAngularSpeed);
    GET_SET_REF(Vect2f, AngularAcceleration);
    GET_SET_REF(Vect3f, StartingSpeed1);
    GET_SET_REF(Vect3f, StartingSpeed2);
    GET_SET_REF(Vect3f, StartingAcceleration1);
    GET_SET_REF(Vect3f, StartingAcceleration2);
    GET_SET_REF(CColor, Color1);
    GET_SET_REF(CColor, Color2);
    GET_SET_PTR(CMaterial, material);

    CMaterial *m_pmaterial;
    int m_NumFrames;
    float m_TimePerFrame;
    bool m_LoopFrames;
    bool m_EmitAbsolute;
    float m_StartingDirectionAngle, m_StartingAccelerationAngle;
    Vect2f m_Size;
    Vect2f m_EmitRate, m_AwakeTime, m_SleepTime, m_Life;
    Vect2f m_StartingAngle, m_StartingAngularSpeed, m_AngularAcceleration;
    Vect3f m_StartingSpeed1, m_StartingSpeed2;
    Vect3f m_StartingAcceleration1, m_StartingAcceleration2;
    CColor m_Color1, m_Color2;

    std::vector<ControlPointColor> m_ControlPointColors;
    std::vector<ControlPointSize> m_ControlPointSizes;

};

#endif //_PARTICLESYSTEMTYPE_20170424
