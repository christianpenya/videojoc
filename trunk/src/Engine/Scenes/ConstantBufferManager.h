#pragma once
#ifndef _ENGINE_CONSTANTBUFFERMANAGER_20170118_H
#define _ENGINE_CONSTANTBUFFERMANAGER_20170118_H

#define MAXBONES 40
#define MAX_OBJECT_RAW_PARAMETER 16
#define MAX_LIGHTS_BY_SHADER 4

#include "Math/Matrix44.h"
#include "ConstantBuffer.h"

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

    enum ConstanBufferVS    { CB_Frame, CB_Object, CB_AnimatedModel, NumConstantBuffersVS };
    enum ConstanBufferPS    { CB_Material, CB_Light, NumConstantBuffersPS };
    PerObjectDesc      mObjDesc;
    PerFrameDesc       mFrameDesc;
    PerAnimatedModelDesc mAnimatedModelDesc;
public:
    CConstantBufferManager()
    {

        // Create all the engine constant buffers, note that we only need to init the size of the buffer, the data will be update for each frame.
        m_VSConstantBuffers[CB_Frame]    = new CConstantBuffer(lRM, sizeof(PerFrameDesc));
        m_VSConstantBuffers[CB_Object]   = new CConstantBuffer(lRM, sizeof(PerObjectDesc));
		m_VSConstantBuffers[CB_AnimatedModel] = new CConstantBuffer(lRM, sizeof(PerAnimatedModelDesc));
        // TODO: Create PS buffers
        m_PSConstantBuffers[CB_Material] = new CConstantBuffer(lRM, sizeof(Mat44f));
        m_PSConstantBuffers[CB_Light]    = new CConstantBuffer(lRM, sizeof(Mat44f));
    }
    virtual void BindVSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID)
    {
        // Update the data of the buffer        
CConstantBuffer* lConstantBuffer = m_VSConstantBuffers[aConstantBufferID];

        void *lRawData = nullptr;
        switch (aConstantBufferID)
        {
        case CB_Frame:
            lRawData = &mFrameDesc;
            break;
        case CB_Object:
            lRawData = &mObjDesc;
            break;
        case CB_AnimatedModel:
            lRawData = &mAnimatedModelDesc;
            break;
        }
        lConstantBuffer->Update(aContext, lRawData);
        lConstantBuffer->BindVS(aContext, aConstantBufferID);
    }

    virtual void BindPSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID)
    {
        CConstantBuffer* lConstantBuffer = m_PSConstantBuffers[aConstantBufferID];
        // TODO: Update constant buffers of PS
        lConstantBuffer->BindPS(aContext, aConstantBufferID);
    }

    virtual ~CConstantBufferManager()  {  }

protected:
    CConstantBuffer* m_VSConstantBuffers[NumConstantBuffersVS];
    CConstantBuffer* m_PSConstantBuffers[NumConstantBuffersPS];
};

#endif //_ENGINE_CONSTANTBUFFERMANAGER_20170118_H