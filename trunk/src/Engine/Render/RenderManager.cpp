#include "RenderManager.h"
#include "Utils/Logger.h"
#include <string>
#include "Utils/CheckedDelete.h"

CRenderManager::CRenderManager():
    m_DebugVertexBufferCurrentIndex(nullptr),
    m_AxisRenderableVertexs(nullptr),
    m_GridRenderableVertexs(nullptr),
    m_CubeRenderableVertexs(nullptr),
    m_SphereRenderableVertexs(nullptr),
    m_NumVerticesAxis(0),
    m_NumVerticesGrid(0),
    m_NumVerticesCube(0),
    m_NumVerticesSphere(0),
    m_Frustum(nullptr),
    m_NumViews(0),
    m_CurrentDepthStencilView(nullptr)
{
}

CRenderManager::~CRenderManager()
{
    ReleasePointers();
    base::utils::CheckedDelete(m_Frustum);
}

void CRenderManager::ReleasePointers()
{
    m_D3DDebug.reset(nullptr);
    m_Device.reset(nullptr);
    m_DeviceContext.reset(nullptr);
    m_SwapChain.reset(nullptr);
    m_RenderTargetView.reset(nullptr);
    m_DepthStencil.reset(nullptr);
    m_DepthStencilView.reset(nullptr);
    m_SolidRenderState.reset(nullptr);

    m_DebugVertexBuffer.reset(nullptr);
    m_DebugVertexShader.reset(nullptr);
    m_DebugVertexLayout.reset(nullptr);
    m_DebugPixelShader.reset(nullptr);
};

void CRenderManager::Init(HWND hWnd, int Width, int Height, bool debugD3D=false)
{
    InitDevice_SwapChain_DeviceContext(hWnd, Width, Height, debugD3D);
    Get_RendertargetView();
    Create_DepthStencil(hWnd, Width, Height);
    SetViewport((float)Width, (float)Height);
    SetRendertarget();

    SetViewProjectionMatrix(m_ViewMatrix, m_ProjectionMatrix);
    CreateDebugShader();

    m_BackgroundColor = { 1, 0, 1, 1 };
    m_SphereOffset(0.0f, 0.0f, 0.0f);

    CreateDebugObjects();

    m_Frustum = new CFrustum();
}

bool CRenderManager::InitDevice_SwapChain_DeviceContext(HWND hWnd, int Width, int Height, bool debugD3D)
{

    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = Width;
    sd.BufferDesc.Height = Height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;

    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    ID3D11Device					*l_Device;
    ID3D11DeviceContext				*l_DeviceContext;
    IDXGISwapChain					*l_SwapChain;

    if (debugD3D)
    {
        if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, debugD3D ? D3D11_CREATE_DEVICE_DEBUG : 0, featureLevels, numFeatureLevels,
                   D3D11_SDK_VERSION, &sd, &l_SwapChain, &l_Device, NULL, &l_DeviceContext)))
        {
            debugD3D = false;
            if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels,
                       D3D11_SDK_VERSION, &sd, &l_SwapChain, &l_Device, NULL, &l_DeviceContext)))
            {
                return FALSE;
            }
        }
    }
    else
    {
        if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels,
                   D3D11_SDK_VERSION, &sd, &l_SwapChain, &l_Device, NULL, &l_DeviceContext)))
        {
            return false;
        }
    }

    m_Device.reset(l_Device);
    m_DeviceContext.reset(l_DeviceContext);
    m_SwapChain.reset(l_SwapChain);

    if (debugD3D)
    {
        ID3D11Debug *l_D3DDebug;
        HRESULT hr = m_Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&l_D3DDebug));
        if (SUCCEEDED(hr))
        {

            debugD3D = false;
        }
        else
        {
            m_D3DDebug.reset(l_D3DDebug);
        }
    }
    return true;
}



bool CRenderManager::Get_RendertargetView()
{
    ID3D11Texture2D *pBackBuffer;
    ID3D11RenderTargetView		*l_RenderTargetView;
    if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        return FALSE;
    //auto rtv = m_RenderTargetView.get();
    HRESULT hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &l_RenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return FALSE;
    m_RenderTargetView.reset(l_RenderTargetView);

    return true;
}

bool CRenderManager::Create_DepthStencil(HWND hWnd, int Width, int Height)
{
    HRESULT hr;
    // Definimos el lienzo
    ID3D11Texture2D *pBackBuffer;
    auto sc = m_SwapChain.get();
    if (FAILED(sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
    {
        return false;
    }
    ID3D11RenderTargetView *l_RenderTargetView;
    hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &l_RenderTargetView);
    m_RenderTargetView.reset(l_RenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = Width;
    descDepth.Height = Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    //auto ds = m_DepthStencil.get();
    ID3D11Texture2D					*l_DepthStencil;
    hr = m_Device->CreateTexture2D(&descDepth, NULL, &l_DepthStencil);

    if (FAILED(hr))
        return false;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    //	auto ds = m_DepthStencil.get();
    //auto dsv = m_DepthStencilView.get();
    ID3D11DepthStencilView			*l_DepthStencilView;
    hr = m_Device->CreateDepthStencilView(l_DepthStencil, &descDSV, &l_DepthStencilView);

    if (FAILED(hr))
        return false;

    m_DepthStencil.reset(l_DepthStencil);
    m_DepthStencilView.reset(l_DepthStencilView);

    D3D11_VIEWPORT vp;
    vp.Width = (float) Width;
    vp.Height = (float) Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_DeviceContext->RSSetViewports(1, &vp);

    // Activamos el render target
    auto dsv = m_DepthStencilView.get();
    auto rtv = m_RenderTargetView.get();
    m_DeviceContext->OMSetRenderTargets(1, &l_RenderTargetView, l_DepthStencilView);

    return true;
}

void CRenderManager::SetViewport(float Width, float Height)
{
    m_Viewport.Width = Width;
    m_Viewport.Height = Height;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

void CRenderManager::SetRendertarget()
{
    auto dsv = m_DepthStencilView.get();
    auto rtv = m_RenderTargetView.get();
    m_DeviceContext->OMSetRenderTargets(1, &rtv, dsv);
}

bool CRenderManager::CreateDebugShader()
{

// C++ macros are nuts
#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)

    const char debugRenderEffectCode[] =
        "#line " TOSTRING(__LINE__) "\n"
        "struct VS_OUTPUT\n"
        "{\n"
        "	float4 Pos : SV_POSITION;\n"
        "	float4 Color : COLOR0;\n"
        "};\n"
        "\n"
        "VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)\n"
        "{\n"
        "	VS_OUTPUT l_Output = (VS_OUTPUT)0;\n"
        "	l_Output.Pos = Pos;\n"
        "	l_Output.Color = Color;\n"
        "	return l_Output;\n"
        "}\n"
        "\n"
        "float4 PS(VS_OUTPUT IN) : SV_Target\n"
        "{\n"
        "	//return float4(1,0,0,1);\n"
        "	//return m_BaseColor;\n"
        "	return IN.Color;\n"
        "}\n";

    ID3DBlob *vsBlob, *psBlob;
    ID3DBlob *errorBlob;

    HRESULT hr = D3DCompile(debugRenderEffectCode, sizeof(debugRenderEffectCode), __FILE__, nullptr, nullptr,
                            "VS", "vs_4_0", 0, 0, &vsBlob, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob != NULL)
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        if (errorBlob)
            errorBlob->Release();
        return false;
    }

    hr = D3DCompile(debugRenderEffectCode, sizeof(debugRenderEffectCode), __FILE__, nullptr, nullptr,
                    "PS", "ps_4_0", 0, 0, &psBlob, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob != NULL)
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        if (errorBlob)
            errorBlob->Release();
        return false;
    }
    //auto dvs = m_DebugVertexShader.get();
    ID3D11VertexShader *l_DebugVertexShader;
    hr = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &l_DebugVertexShader);
    m_DebugVertexShader.reset(l_DebugVertexShader);

    if (FAILED(hr))
        return false;
    ID3D11PixelShader *l_DebugPixelShader;
    //auto dpl = m_DebugPixelShader.get();
    hr = m_Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &l_DebugPixelShader);
    m_DebugPixelShader.reset(l_DebugPixelShader);

    if (FAILED(hr))
        return false;

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    ID3D11InputLayout *l_DebugVertexLayout;
    //auto dvl = m_DebugVertexLayout.get();
    hr = m_Device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &l_DebugVertexLayout);
    m_DebugVertexLayout.reset(l_DebugVertexLayout);
    if (FAILED(hr))
        return false;

    D3D11_BUFFER_DESC l_BufferDescription;
    ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
    l_BufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    l_BufferDescription.ByteWidth = sizeof(CDebugVertex)*DebugVertexBufferSize;
    l_BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    l_BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //D3D11_SUBRESOURCE_DATA InitData;
    //ZeroMemory(&InitData, sizeof(InitData));
    //InitData.pSysMem = Vtxs;
    //auto dvb = m_DebugVertexBuffer.get();
    ID3D11Buffer *l_DebugVertexBuffer;
    hr = m_Device->CreateBuffer(&l_BufferDescription, nullptr, &l_DebugVertexBuffer);
    m_DebugVertexBuffer.reset(l_DebugVertexBuffer);
    if (FAILED(hr))
        return false;

    return true;
}

void CRenderManager::Draw_Triangle()
{
    CDebugVertex resultBuffer[] =
    {
        { Vect4f(-0.5f, -0.5f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vect4f(+0.0f, +0.5f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vect4f(+0.5f, -0.5f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) }
    };

    // set vertex data
    D3D11_MAPPED_SUBRESOURCE resource;
    auto dvb = m_DebugVertexBuffer.get();
    HRESULT hr = m_DeviceContext->Map(dvb, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

    if (FAILED(hr))
        return; // TODO log

#ifdef _DEBUG
    assert(3 * sizeof(CDebugVertex) < DebugVertexBufferSize * sizeof(CDebugVertex));
#endif

    memcpy(resource.pData, resultBuffer, 3 * sizeof(CDebugVertex));
    m_DeviceContext->Unmap(dvb, 0);

    UINT stride = sizeof(CDebugVertex);
    UINT offset = 0;
    m_DeviceContext->IASetVertexBuffers(0, 1, &dvb, &stride, &offset);
    m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    auto dvl = m_DebugVertexLayout.get();
    auto dvs = m_DebugVertexShader.get();
    auto dps = m_DebugPixelShader.get();
    m_DeviceContext->IASetInputLayout(dvl);
    m_DeviceContext->VSSetShader(dvs, NULL, 0);
    m_DeviceContext->PSSetShader(dps, NULL, 0);

    m_DeviceContext->Draw(3, 0);
}

void CRenderManager::CreateDebugObjects()
{
    //AXIS
    static CDebugVertex l_AxisVtxs[6] =
    {
        { Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vect4f(1.0f, 0.0f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

        { Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vect4f(0.0f, 1.0f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

        { Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vect4f(0.0f, 0.0f, 1.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    m_AxisRenderableVertexs = l_AxisVtxs;
    m_NumVerticesAxis = 6;

    //CUBE
    const float l_SizeCube = 1.0f;
    static CDebugVertex l_CubeVtxs[] =
    {
        // vertex 1
        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        // vertex 2
        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        // vertex 3
        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },


        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        // vertex 4
        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

        { Vect4f(-l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vect4f(l_SizeCube / 2.0f, l_SizeCube / 2.0f, l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },

    };

    m_CubeRenderableVertexs = l_CubeVtxs;
    m_NumVerticesCube = 8 * 3;

    //GRID
    float l_Size = 10.0f;
    const int l_Grid = 10;
    static CDebugVertex l_GridVtxs[(l_Grid + 1) * 2 * 2];
    float axis = 0;

    //LINEAS EN Z
    for (int b = 0; b <= l_Grid; ++b)
    {

        axis = (l_Size / 2.0f) * (((2.0f*b) / l_Size) - 1.0f);

        l_GridVtxs[b * 2].Position = Vect4f(axis, 0, -l_Size / 2.0f, 1.0f);
        l_GridVtxs[b * 2].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
        l_GridVtxs[(b * 2) + 1].Position = Vect4f(axis, 0, l_Size / 2.0f, 1.0f);
        l_GridVtxs[(b * 2) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    //LINEAS EN X
    for (int b = 0; b <= l_Grid; ++b)
    {

        axis = (l_Size / 2.0f) * (((2.0f*b) / l_Size) - 1.0f);

        l_GridVtxs[(l_Grid + 1) * 2 + (b * 2)].Position = Vect4f(-l_Size / 2.0f, 0, axis, 1.0f);
        l_GridVtxs[(l_Grid + 1) * 2 + (b * 2)].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
        l_GridVtxs[(l_Grid + 1) * 2 + (b * 2) + 1].Position = Vect4f(l_Size / 2.0f, 0, axis, 1.0f);
        l_GridVtxs[(l_Grid + 1) * 2 + (b * 2) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    m_GridRenderableVertexs = l_GridVtxs;
    m_NumVerticesGrid = (l_Grid + 1) * 2 * 2;

    //SPHERE
    const int l_Aristas = 10;
    static CDebugVertex l_SphereVtxs[4 * l_Aristas*l_Aristas];

    for (int t = 0; t<l_Aristas; ++t)
    {

        float l_RadiusRing = sin(/*DEG2RAD*/(6.28318531f / 360.f) * (180.0f*((float)t)) / ((float)l_Aristas));

        for (int b = 0; b<l_Aristas; ++b)
        {

            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 0].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.x, cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)) + m_SphereOffset.y, l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.z, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 0].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 1].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)(b + 1)) / ((float)l_Aristas))) + m_SphereOffset.x, cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)) + m_SphereOffset.y, l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)(b + 1)) / ((float)l_Aristas))) + m_SphereOffset.z, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);

            float l_RadiusNextRing = sin(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)(t + 1))) / ((float)l_Aristas));

            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 2].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.x, cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)) + m_SphereOffset.y, l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.z, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 2].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 3].Position = Vect4f(l_RadiusNextRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.x, cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)(t + 1))) / ((float)l_Aristas)) + m_SphereOffset.y, l_RadiusNextRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))) + m_SphereOffset.z, 1.0f);
            l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 3].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    m_SphereRenderableVertexs = l_SphereVtxs;
    m_NumVerticesSphere = 4 * l_Aristas*l_Aristas;
}

void CRenderManager::SetModelMatrix(const Mat44f &Model)
{
    m_ModelMatrix = Model;
    m_ModelViewProjectionMatrix = m_ModelMatrix * m_ViewProjectionMatrix;
}

void CRenderManager::SetViewMatrix(const Mat44f &View)
{
    m_ViewMatrix = View;
    m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    m_ModelViewProjectionMatrix = m_ModelMatrix * m_ViewProjectionMatrix;
}

void CRenderManager::SetProjectionMatrix(const Mat44f &Projection)
{
    m_ProjectionMatrix = Projection;
    m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    m_ModelViewProjectionMatrix = m_ModelMatrix * m_ViewProjectionMatrix;
}

void CRenderManager::SetViewProjectionMatrix(const Mat44f &View, const Mat44f &Projection)
{
    m_ViewMatrix = View;
    m_ProjectionMatrix = Projection;
    m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    m_ModelViewProjectionMatrix = m_ModelMatrix * m_ViewProjectionMatrix;
}

void CRenderManager::SetViewMatrix(const Vect3f& vPos, const Vect3f& vTarget, const Vect3f& vUp)
{
    Mat44f View;
    View.SetIdentity();
    View.SetFromLookAt(vPos, vTarget, vUp);
    SetViewMatrix(View);
}

void CRenderManager::SetProjectionMatrix(float fovy, float aspect, float zn, float zf)
{
    Mat44f Projection;
    Projection.SetIdentity();
    Projection.SetFromPerspective(fovy, aspect, zn, zf);
    SetProjectionMatrix(Projection);
}

void CRenderManager::DebugRender(const Mat44f& modelViewProj, const CDebugVertex* modelVertices, int numVertices, CColor colorTint)
{
    CDebugVertex *resultBuffer = (CDebugVertex *)alloca(numVertices * sizeof(CDebugVertex));

    for (int i = 0; i < numVertices; ++i)
    {
        resultBuffer[i].Position = (modelVertices[i].Position * modelViewProj);
        resultBuffer[i].Color = modelVertices[i].Color * colorTint;
    }

    // set vertex data
    D3D11_MAPPED_SUBRESOURCE resource;

    auto dvb = m_DebugVertexBuffer.get();
    HRESULT hr = m_DeviceContext->Map(dvb, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

    if (FAILED(hr))
        return; // TODO log

#ifdef _DEBUG
    assert(numVertices * sizeof(CDebugVertex) < DebugVertexBufferSize * sizeof(CDebugVertex));
#endif

    memcpy(resource.pData, resultBuffer, numVertices * sizeof(CDebugVertex));

    //auto dvb = m_DebugVertexBuffer.get();
    m_DeviceContext->Unmap(dvb, 0);


    UINT stride = sizeof(CDebugVertex);
    UINT offset = 0;
    //auto dvb = m_DebugVertexBuffer.get();
    m_DeviceContext->IASetVertexBuffers(0, 1, &dvb, &stride, &offset);
    m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    auto dvl = m_DebugVertexLayout.get();
    auto dvs = m_DebugVertexShader.get();
    auto dps = m_DebugPixelShader.get();
    m_DeviceContext->IASetInputLayout(dvl);
    m_DeviceContext->VSSetShader(dvs, NULL, 0);
    m_DeviceContext->PSSetShader(dps, NULL, 0);

    m_DeviceContext->Draw(numVertices, 0);
}

void CRenderManager::DrawAxis(float SizeX, float SizeY, float SizeZ, const CColor &Color)
{
    Mat44f scale;
    scale.SetIdentity();
    scale.SetScale(SizeX, SizeY, SizeZ);

    Mat44f viewProj = scale * m_ModelViewProjectionMatrix;

    DebugRender(viewProj, m_AxisRenderableVertexs, m_NumVerticesAxis, Color);
}

void CRenderManager::DrawCube(float SizeX, float SizeY, float SizeZ, const CColor &Color)
{
    Mat44f scale;
    scale.SetIdentity();
    scale.SetScale(SizeZ, SizeY, SizeZ);

    Mat44f viewProj = scale * m_ModelViewProjectionMatrix;

    DebugRender(viewProj, m_CubeRenderableVertexs, m_NumVerticesCube, Color);
}

void CRenderManager::DrawGrid(float SizeX, float SizeY, float SizeZ, const CColor &Color)
{
    Mat44f scale;
    scale.SetIdentity();
    scale.SetScale(SizeZ, SizeY, SizeZ);

    Mat44f viewProj = scale * m_ModelViewProjectionMatrix;

    DebugRender(viewProj, m_GridRenderableVertexs, m_NumVerticesGrid, Color);
}

void CRenderManager::DrawSphere(float Radius, const CColor &Color)
{
    Mat44f scale;
    scale.SetIdentity();
    scale.SetScale(Radius, Radius, Radius);

    Mat44f viewProj = scale * m_ModelViewProjectionMatrix;

    DebugRender(viewProj, m_SphereRenderableVertexs, m_NumVerticesSphere, Color);
}

void CRenderManager::Resize(float Width, float Height, HWND hWnd)
{
    if (m_Device.get() != nullptr)
    {
        m_DeviceContext.get()->OMSetRenderTargets(0, nullptr, nullptr);

        m_RenderTargetView.reset(nullptr);
        m_DepthStencil.reset(nullptr);
        m_DepthStencilView.reset(nullptr);

        auto l_SwapChain = m_SwapChain.get();
        l_SwapChain->ResizeBuffers(0, (UINT)Width, (UINT) Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        Create_DepthStencil(hWnd, (int) Width, (int) Height);
        SetProjectionMatrix(45.0f, (float)Width / (float)Height, 0.5f, 100.0f);
    }
}

void CRenderManager::ClearAltIntro(HWND hWnd)
{
    IDXGIFactory* dxgiFactory;
    HRESULT hr = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);

#ifdef _DEBUG
    assert(hr == S_OK);
#endif

    hr = dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

#ifdef _DEBUG
    assert(hr == S_OK);
#endif

    dxgiFactory->Release();
}

void CRenderManager::ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition)
{
    // This follows Raymond Chen's prescription
    // for fullscreen toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

    DWORD Style = GetWindowLongW(Window, GWL_STYLE);
    if (Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
        if (GetWindowPlacement(Window, &WindowPosition) &&
                GetMonitorInfoW(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLongW(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(Window, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLongW(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Window, &WindowPosition);
        SetWindowPos(Window, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

void CRenderManager::ReportLive()
{
    if (m_D3DDebug)
        m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);

}

// Rendering
void CRenderManager::BeginRender()
{
    auto rtv = m_RenderTargetView.get();
    auto dsv = m_DepthStencilView.get();
    m_DeviceContext->ClearRenderTargetView(rtv, &m_BackgroundColor.x);
    m_DeviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    this->SetSolidRenderState();
}

void CRenderManager::SetSolidRenderState()
{
    auto srs = m_SolidRenderState.get();
    m_DeviceContext->RSSetState(srs);
}

void CRenderManager::EndRender()
{
    m_SwapChain->Present(1, 0);
}

void CRenderManager::Clear(bool renderTarget, bool depthStencil, const CColor& backgroundColor)
{
    if (renderTarget)
    {
        for (int i = 0; i < m_NumViews; ++i)
        {
            if (m_CurrentRenderTargetViews[i] != 0)
            {
                m_DeviceContext->ClearRenderTargetView(m_CurrentRenderTargetViews[i], &backgroundColor.x);
            }
        }
    }
    else
    {
        m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.get(), &backgroundColor.x);
    }
    // TODO: Handle the current render target view stencil and depth
    if (depthStencil)

        m_DeviceContext->ClearDepthStencilView(m_CurrentDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderManager::SetViewport(const Vect2u & aPosition, const Vect2u & aSize) const
{
    D3D11_VIEWPORT l_Viewport;
    l_Viewport.Width = aSize.x;
    l_Viewport.Height = aSize.y;
    l_Viewport.MinDepth = 0.0f;
    l_Viewport.MaxDepth = 1.0f;
    l_Viewport.TopLeftX = aPosition.x;
    l_Viewport.TopLeftY = aPosition.y;
    m_DeviceContext->RSSetViewports(1, &l_Viewport);
}

void CRenderManager::ResetViewport() const
{
    m_DeviceContext->RSSetViewports(1, &m_Viewport);
}


void CRenderManager::UnsetRenderTargets()
{
    ID3D11RenderTargetView * lTargetView = m_RenderTargetView.get();
    SetRenderTargets(1, &lTargetView, m_DepthStencilView.get());
    ResetViewport();
}

void CRenderManager::SetRenderTargets(int aNumViews, ID3D11RenderTargetView* const* aRenderTargetViews, ID3D11DepthStencilView *aDepthStencilViews)
{
    m_NumViews = aNumViews;

    for (int i = 0; i<MAX_RENDER_TARGETS; ++i)
    {
        if (i<m_NumViews)
            m_CurrentRenderTargetViews[i] = aRenderTargetViews[i];
        else
            m_CurrentRenderTargetViews[i] = nullptr;
    }
    if (aDepthStencilViews)
    {
        m_CurrentDepthStencilView = aDepthStencilViews;
        m_DeviceContext->OMSetRenderTargets(MAX_RENDER_TARGETS, &m_CurrentRenderTargetViews[0], aDepthStencilViews);
    }
    else
    {
        m_CurrentDepthStencilView = m_DepthStencilView.get();
        m_DeviceContext->OMSetRenderTargets(MAX_RENDER_TARGETS, &m_CurrentRenderTargetViews[0], m_DepthStencilView.get());
    }
}

void CRenderManager::Update()
{
    m_Frustum->Update(m_ViewProjectionMatrix);
}

Vect2u CRenderManager::GetWindowSize()
{
    return Vect2u(m_Viewport.Width, m_Viewport.Height);
}
