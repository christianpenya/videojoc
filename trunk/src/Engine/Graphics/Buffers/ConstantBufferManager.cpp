#include "ConstantBufferManager.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "Utils/CheckedDelete.h"

CConstantBufferManager::CConstantBufferManager()
{
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();

    // Create all the engine constant buffers, note that we only need to init the size of the buffer, the data will be update for each frame.
    m_ConstantBuffers[CB_Frame] = new CConstantBuffer(lRM, sizeof(PerFrameDesc));
    m_ConstantBuffers[CB_Object] = new CConstantBuffer(lRM, sizeof(PerObjectDesc));
    m_ConstantBuffers[CB_AnimatedModel] = new CConstantBuffer(lRM, sizeof(PerAnimatedModelDesc));
    m_ConstantBuffers[CB_Light] = new CConstantBuffer(lRM, sizeof(PerLightsDesc));

    m_ConstantBuffers[CB_Material] = new CConstantBuffer(lRM, sizeof(PerMaterialDesc));
}

CConstantBufferManager::~CConstantBufferManager()
{
    base::utils::CheckedDelete(m_ConstantBuffers[CB_Frame]);
    base::utils::CheckedDelete(m_ConstantBuffers[CB_Object]);
    base::utils::CheckedDelete(m_ConstantBuffers[CB_AnimatedModel]);
    base::utils::CheckedDelete(m_ConstantBuffers[CB_Light]);
    base::utils::CheckedDelete(m_ConstantBuffers[CB_Material]);
}

void CConstantBufferManager::BindBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID)
{
    // Update the data of the buffer
    CConstantBuffer* lConstantBuffer = m_ConstantBuffers[aConstantBufferID];

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
    case CB_Light:
        lRawData = &mLightsDesc;
        break;
    case CB_Material:
        lRawData = &mMaterialDesc;
        break;
    }

    lConstantBuffer->Update(aContext, lRawData);
    lConstantBuffer->BindVS(aContext, aConstantBufferID);
    lConstantBuffer->BindPS(aContext, aConstantBufferID);
    lConstantBuffer->BindGS(aContext, aConstantBufferID);
}
