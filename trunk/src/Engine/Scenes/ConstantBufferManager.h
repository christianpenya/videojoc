#pragma once
#ifndef _ENGINE_CONSTANTBUFFERMANAGER_20170118_H
#define _ENGINE_CONSTANTBUFFERMANAGER_20170118_H

#define MAXBONES 40
#define MAX_OBJECT_RAW_PARAMETER 16
#define MAX_LIGHTS_BY_SHADER 4

#include "Math/Matrix44.h"
#include "ConstantBuffer.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"

struct PerFrameDesc
{
    Mat44f m_View;
    Mat44f m_Projection;
    Mat44f m_ViewProjection;
};

struct PerLightsDesc
{
    Vect4f m_LightAmbient;
    float m_LightEnabled[MAX_LIGHTS_BY_SHADER];
    float m_LightType[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightPosition[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightDirection[MAX_LIGHTS_BY_SHADER];
    float m_LightAngle[MAX_LIGHTS_BY_SHADER];
    float m_LightFallOffAngle[MAX_LIGHTS_BY_SHADER];
    float m_LightAttenuationStartRange[MAX_LIGHTS_BY_SHADER];
    float m_LightAttenuationEndRange[MAX_LIGHTS_BY_SHADER];
    float m_LightIntensity[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightColor[MAX_LIGHTS_BY_SHADER];
};

struct PerObjectDesc
{
    Mat44f m_World;
};

struct PerAnimatedModelDesc
{
    Mat44f m_Bones[MAXBONES];
};

struct PerMaterialDesc
{
    Vect4f m_RawData[MAX_OBJECT_RAW_PARAMETER];
};

class CConstantBufferManager
{
public:

    enum ConstanBufferVS    { CB_Frame, CB_Object, CB_AnimatedModel, CB_LightVS, NumConstantBuffersVS };
    enum ConstanBufferPS    { CB_Material, CB_LightPS, NumConstantBuffersPS };
    PerObjectDesc      mObjDesc;
    PerFrameDesc       mFrameDesc;
    PerAnimatedModelDesc mAnimatedModelDesc;
    PerMaterialDesc mMaterialDesc;
    PerLightsDesc mLightsDesc;
public:
    CConstantBufferManager();
    virtual ~CConstantBufferManager();

    virtual void BindVSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID);

    virtual void BindPSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID);




protected:
    CConstantBuffer* m_VSConstantBuffers[NumConstantBuffersVS];
    CConstantBuffer* m_PSConstantBuffers[NumConstantBuffersPS];
};

#endif //_ENGINE_CONSTANTBUFFERMANAGER_20170118_H