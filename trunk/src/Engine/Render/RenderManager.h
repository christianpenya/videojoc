#pragma once

#ifndef _ENGINE_RENDERMANAGER_CPB_2016110320428_H
#define _ENGINE_RENDERMANAGER_CPB_2016110320428_H

#include "Utils\Release.h"
#include <Windows.h>
#include <winerror.h>
#include "d3dcompiler.h"
#include "d3d11.h"
#include "dxgi.h"
#include "Math/Color.h"
#include "Math/Matrix44.h"
#include "Graphics/Camera/Frustum.h"
#define MAX_RENDER_TARGETS 5


#ifdef _DEBUG
#include <assert.h>
#include "Utils/MemLeaks/MemLeaks.h"
#endif

// Render Manager Class
class CRenderManager
{

public:
    struct CDebugVertex
    {
        Vect4f Position;
        CColor Color;
    };

    static const int DebugVertexBufferSize = 0x10000;
    Vect3f  m_SphereOffset;

protected:
    releaser_ptr<ID3D11Device>						m_Device;
    releaser_ptr<ID3D11DeviceContext>				m_DeviceContext;
    releaser_ptr<IDXGISwapChain>					m_SwapChain;
    releaser_ptr<ID3D11RenderTargetView>			m_RenderTargetView;
    releaser_ptr<ID3D11Texture2D>					m_DepthStencil;
    releaser_ptr<ID3D11DepthStencilView>			m_DepthStencilView;
    releaser_ptr<ID3D11Debug>						m_D3DDebug;

    releaser_ptr<ID3D11Buffer	>						m_DebugVertexBuffer;
    int										*m_DebugVertexBufferCurrentIndex;
    releaser_ptr<ID3D11VertexShader	>					m_DebugVertexShader;
    releaser_ptr<ID3D11InputLayout	>					m_DebugVertexLayout;
    releaser_ptr<ID3D11PixelShader	>					m_DebugPixelShader;

    const CDebugVertex*						m_AxisRenderableVertexs;
    const CDebugVertex*						m_GridRenderableVertexs;
    const CDebugVertex*						m_CubeRenderableVertexs;
    const CDebugVertex*						m_SphereRenderableVertexs;

    int m_NumVerticesAxis, m_NumVerticesGrid, m_NumVerticesCube, m_NumVerticesSphere;

    releaser_ptr<ID3D11RasterizerState>			m_SolidRenderState;

    Mat44f m_ViewProjectionMatrix, m_ModelViewProjectionMatrix;

    void DebugRender(const Mat44f& modelViewProj, const CDebugVertex* modelVertices, int numVertices, CColor colorTint);

public:
    Vect4f									m_BackgroundColor;

public:
    CRenderManager();
    ~CRenderManager();

    void ReleasePointers();

    void Init(HWND hWnd, int Width, int Height, bool debugD3D);
    bool InitDevice_SwapChain_DeviceContext(HWND hWnd, int Width, int Height, bool debugD3D);
    bool Get_RendertargetView();
    bool Create_DepthStencil(HWND hWnd, int Width, int Height);
    void SetViewport(float Width, float Height);
    void SetRendertarget();
    void ReportLive();

    bool CreateDebugShader();

    void Destroy();

    // Rendering
    void BeginRender();
    void SetSolidRenderState();
    void EndRender();

    // Drawing
    void DrawAxis(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
    void DrawGrid(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
    void DrawCube(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
    void DrawSphere(float Radius, const CColor &Color = colWHITE);
    void DrawSphere(float Radius, float Offset, const CColor &Color = colWHITE);

    void DrawAxis(Vect3f Size, const CColor &Color = colWHITE)
    {
        DrawAxis(Size.x, Size.y, Size.z, Color);
    }
    void DrawAxis(float Size = 1.0f, const CColor &Color = colWHITE)
    {
        DrawAxis(Size, Size, Size, Color);
    }
    void DrawGrid(Vect3f Size, const CColor &Color = colWHITE)
    {
        DrawGrid(Size.x, Size.y, Size.z, Color);
    }
    void DrawGrid(float Size = 1.0f, const CColor &Color = colWHITE)
    {
        DrawGrid(Size, Size, Size, Color);
    }
    void DrawCube(Vect3f Size, const CColor &Color = colWHITE)
    {
        DrawCube(Size.x, Size.y, Size.z, Color);
    }
    void DrawCube(float Size = 1.0f, const CColor &Color = colWHITE)
    {
        DrawCube(Size, Size, Size, Color);
    }

    void Draw_Triangle();
    void CreateDebugObjects();

    // Matrix Operations
    void SetModelMatrix(const Mat44f &Model);

    void SetViewMatrix(const Mat44f &View);
    Mat44f GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    void SetProjectionMatrix(const Mat44f &Projection);
    Mat44f GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    void SetViewProjectionMatrix(const Mat44f &View, const Mat44f &Projection);
    Mat44f GetViewProjectionMatrix() const
    {
        return m_ViewProjectionMatrix;
    }

    void SetViewMatrix(const Vect3f& vPos, const Vect3f& vTarget, const Vect3f& vUp);
    void SetProjectionMatrix(float fovy, float aspect, float zn, float zf);

    ID3D11Device* GetDevice() const
    {
        return m_Device.get();
    }
    ID3D11DeviceContext* GetDeviceContext() const
    {
        return m_DeviceContext.get();
    }
    void Resize(float Width, float Height, HWND hWnd);
    void ClearAltIntro(HWND hWnd);
    void ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition);

    void Clear(bool renderTarget, bool depthStencil, const CColor& backgroundColor);
    void SetViewport(const Vect2u & aPosition, const Vect2u & aSize) const;
    void ResetViewport() const;
    void UnsetRenderTargets();
    void SetRenderTargets(int aNumViews, ID3D11RenderTargetView* const* aRenderTargetViews, ID3D11DepthStencilView *aDepthStencilViews);

    void Update();

    Vect2u GetWindowSize();

    Mat44f m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix; //#TODO, tienen getter y setter publico, no se pueden devolver a private? Render pipeline.

    CFrustum* m_Frustum;

private:
    ID3D11RenderTargetView* m_CurrentRenderTargetViews[MAX_RENDER_TARGETS];
    int m_NumViews;
    ID3D11DepthStencilView* m_CurrentDepthStencilView;
    D3D11_VIEWPORT m_Viewport;
};

#endif //_ENGINE_RENDERMANAGER_CPB_2016110320428_H
