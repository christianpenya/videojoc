#pragma once
#ifndef _PARTICLESYSTEMINSTANCE_H
#define _PARTICLESYSTEMINSTANCE_H

#define s_MaxParticlesPerInstance 200

#include "Math/Matrix44.h"
#include "Math/Color.h"
#include "XML\XML.h"
#include "Utils/Logger.h"
#include "ParticleSystemType.h"
#include "Graphics/Mesh/VertexTypes.h"
#include "Graphics/Mesh/TemplatedGeometry.h"
#include "Graphics/Mesh/VertexBuffer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "d3dcompiler.h"
#include "d3d11.h"
#include <random>

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CParticleSystemType;
class CParticleSystemInstance : public CSceneNode
{
private:
    struct ParticleData
    {
        Vect3f Position, Velocity, Acceleration;
        int CurrentFrame;
        float TimeToNextFrame=0;
        float Lifetime = 0;
        float TotalLife=0;
        float Angle, AngularSpeed, AngularAcceleration;
        int ColorControlPoint, SizeControlPoint;
        float LastColorControlTime, NextColorControlTime;
        float LastSizeControlTime, NextSizeControlTime;
        CColor LastColor, NextColor;
        float LastSize, NextSize;
        float DistanceToCamera;
    };

    ParticleData m_ParticleData[s_MaxParticlesPerInstance];
    CGeometryPointList<VertexTypes::ParticleVertex> * m_Vertices;

public:
    CParticleSystemInstance();
    virtual ~CParticleSystemInstance();
    CParticleSystemInstance(const CXMLElement* aElement);

    float ComputeTimeToNextParticle();
    float GetDistanceToCamera(Vect3f particlePosition);

    bool Render(CRenderManager& lRM);
    bool Update(float ElapsedTime);

    void UpdateState(float ElapsedTime);
    void orderParticles(ParticleData arr[], int length);

    CParticleSystemType *m_Type;
    float m_NextParticleEmission;
    bool m_Awake;
    float m_AwakeTimer;
    Vect3f m_EmissionBoxHalfSize;
    float m_EmissionVolume, m_EmissionScaler;
    int m_ActiveParticles;
    //void DrawImgui();
    void Deactivate();

    VertexTypes::ParticleVertex m_ParticleRenderableData[s_MaxParticlesPerInstance];
    std::mt19937 m_RandomEngine;
    std::uniform_real_distribution<float> m_UnitDistribution;
    std::random_device rnd;
private:
    float GetRandomValue(float min, float max);
    Vect3f GetRandomValue(Vect3f min, Vect3f max);
    Vect2f GetRandomValue(Vect2f min, Vect2f max);
    CColor GetRandomValue(CColor min, CColor max);
    float GetRandomValue(Vect2f value);

};


#endif //_PARTICLESYSTEMINSTANCE_H