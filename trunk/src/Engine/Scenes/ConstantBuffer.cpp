#include "ConstantBuffer.h"

CConstantBuffer::CConstantBuffer(CRenderManager& RenderManager, uint32 aByteWidth) : CBuffer()
{
    assert(aByteWidth % 16 == 0);
    ID3D11Device * lDevice = RenderManager.GetDevice();

    // Create the constant buffers for the variables defined in the vertex shader.
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = aByteWidth;
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    HRESULT lHR = lDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_pBuffer);
    LOG_ERROR_APPLICATION_IF(FAILED(lHR), "Error creating constant buffer");
}

CConstantBuffer::CConstantBuffer(CRenderManager& RenderManager, void* aRawData, uint32 aByteWidth) : CBuffer()
{
    assert(aByteWidth % 16 == 0);

    // Fill in a buffer description.
    D3D11_BUFFER_DESC lConstantBuffer;
    ZeroMemory(&lConstantBuffer, sizeof(lConstantBuffer));

    lConstantBuffer.Usage = D3D11_USAGE_IMMUTABLE;
    lConstantBuffer.CPUAccessFlags = 0;
    lConstantBuffer.ByteWidth = aByteWidth;
    lConstantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lConstantBuffer.MiscFlags = 0;
    lConstantBuffer.StructureByteStride = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = aRawData;
    m_RawData = aRawData;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the buffer.
    ID3D11Device *lDevice = RenderManager.GetDevice();
    HRESULT lHR = lDevice->CreateBuffer(&lConstantBuffer, &InitData, &m_pBuffer);
    LOG_ERROR_APPLICATION_IF(FAILED(lHR), "Error creating immutable constant buffer");
}

CConstantBuffer::~CConstantBuffer() {}

void CConstantBuffer::Bind(ID3D11DeviceContext* aContext)
{
    assert("This method must not be called!");
}

void CConstantBuffer::BindVS(ID3D11DeviceContext* aContext, uint32 aBufferID)
{
    aContext->VSSetConstantBuffers(aBufferID, 1, &m_pBuffer);
}

void CConstantBuffer::BindPS(ID3D11DeviceContext* aContext, uint32 aBufferID)
{
    aContext->PSSetConstantBuffers(aBufferID, 1, &m_pBuffer);
}

void CConstantBuffer::Update(ID3D11DeviceContext* aContext, void* aRawData)
{
    aContext->UpdateSubresource(m_pBuffer, 0, NULL, aRawData, 0, 0);
}