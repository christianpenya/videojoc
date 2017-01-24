#pragma once
#ifndef _ENGINE_CONSTANTBUFFERMANAGER_20170118_H
#define _ENGINE_CONSTANTBUFFERMANAGER_20170118_H

#include "Math/Matrix44.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "ConstantBuffer.h"

struct PerFrameDesc
{
    Mat44f m_View;
    Mat44f m_Projection;
    Mat44f m_ViewProjection;
};
struct PerObjectDesc
{
    Mat44f m_World;
};

class CConstantBufferManager
{
public:
    enum ConstanBufferVS    { CB_Frame, CB_Object, NumConstantBuffersVS };
    enum ConstanBufferPS    { CB_Material, CB_Light, NumConstantBuffersPS };
    PerObjectDesc      mObjDesc;
    PerFrameDesc       mFrameDesc;
public:
    CConstantBufferManager()
    {
        CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();

        // Create all the engine constant buffers, note that we only need to init the size of the buffer, the data will be update for each frame.
        m_VSConstantBuffers[CB_Frame]    = new CConstantBuffer(lRM, sizeof(PerFrameDesc));
        m_VSConstantBuffers[CB_Object]   = new CConstantBuffer(lRM, sizeof(PerObjectDesc));
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