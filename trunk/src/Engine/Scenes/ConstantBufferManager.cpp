#include "ConstantBufferManager.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"

CConstantBufferManager::CConstantBufferManager()
{
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();

    // Create all the engine constant buffers, note that we only need to init the size of the buffer, the data will be update for each frame.
    m_VSConstantBuffers[CB_Frame] = new CConstantBuffer(lRM, sizeof(PerFrameDesc));
    m_VSConstantBuffers[CB_Object] = new CConstantBuffer(lRM, sizeof(PerObjectDesc));
    m_VSConstantBuffers[CB_AnimatedModel] = new CConstantBuffer(lRM, sizeof(PerAnimatedModelDesc));
    m_VSConstantBuffers[CB_LightVS] = new CConstantBuffer(lRM, sizeof(PerLightsDesc));

    m_PSConstantBuffers[CB_Material] = new CConstantBuffer(lRM, sizeof(PerMaterialDesc));
    m_PSConstantBuffers[CB_LightPS] = new CConstantBuffer(lRM, sizeof(PerLightsDesc));
}

CConstantBufferManager::~CConstantBufferManager() {}

void CConstantBufferManager::BindVSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID)
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
    case CB_LightVS:
        lRawData = &mLightsDesc;
        break;
    }

    lConstantBuffer->Update(aContext, lRawData);
    lConstantBuffer->BindVS(aContext, aConstantBufferID);
}

void CConstantBufferManager::BindPSBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID)
{
    CConstantBuffer* lConstantBuffer = m_PSConstantBuffers[aConstantBufferID];
    void *lRawData = nullptr;

    switch (aConstantBufferID)
    {
    case CB_Material:
        lRawData = &mMaterialDesc;
        break;
    case CB_LightPS:
        lRawData = &mLightsDesc;
        break;
    }

    lConstantBuffer->Update(aContext, lRawData);
    lConstantBuffer->BindPS(aContext, aConstantBufferID);
}