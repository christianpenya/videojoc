#pragma once

#ifndef _ENGINE_RENDERMANAGER_CPB_2016110320428_H
#define _ENGINE_RENDERMANAGER_CPB_2016110320428_H

#include "Base\Utils\Release.h"
#include <Windows.h>
#include <winerror.h>
#include "d3dcompiler.h"
#include "d3d11.h"
#include "dxgi.h"
#include "Math/Color.h"
#include "Math/Matrix44.h"

#ifdef _DEBUG
#include <assert.h>
#endif

// Render Manager Class
class CRenderManager {
		
public:
	struct CDebugVertex {
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

	
	ID3D11Buffer						*m_DebugVertexBuffer;
	int										*m_DebugVertexBufferCurrentIndex;
	ID3D11VertexShader				*m_DebugVertexShader;
	ID3D11InputLayout					*m_DebugVertexLayout;
	ID3D11PixelShader					*m_DebugPixelShader;

	const CDebugVertex*						m_AxisRenderableVertexs;
	const CDebugVertex*						m_GridRenderableVertexs;
	const CDebugVertex*						m_CubeRenderableVertexs;
	const CDebugVertex*						m_SphereRenderableVertexs;

	int m_NumVerticesAxis, m_NumVerticesGrid, m_NumVerticesCube, m_NumVerticesSphere;

	ID3D11RasterizerState					*m_SolidRenderState;

	Mat44f m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix;
	Mat44f m_ViewProjectionMatrix, m_ModelViewProjectionMatrix;

	void DebugRender(const Mat44f& modelViewProj, const CDebugVertex* modelVertices, int numVertices, CColor colorTint);

public:
	Vect4f									m_BackgroundColor;

public:
	CRenderManager(){};
	~CRenderManager(){};

	void Init(HWND hWnd, int Width, int Height);
			
	bool InitDevice_SwapChain_DeviceContext(HWND hWnd, int Width, int Height);
	bool Get_RendertargetView();
	bool Create_DepthStencil(HWND hWnd, int Width, int Height);
	void Set_Viewport(int Width, int Height);
	void SetRendertarget();
			
	bool CreateDebugShader();

	void Destroy();

	// Rendering
	void BeginRender();
	void SetSolidRenderState();
	void DoRender();
	void EndRender();

	// Drawing
	void DrawAxis(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
	void DrawGrid(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
	void DrawCube(float SizeX, float SizeY, float SizeZ, const CColor &Color = colWHITE);
	void DrawSphere(float Radius, const CColor &Color = colWHITE);
	void DrawSphere(float Radius, float Offset, const CColor &Color = colWHITE);

	void DrawAxis(Vect3f Size, const CColor &Color = colWHITE) { DrawAxis(Size.x, Size.y, Size.z, Color); }
	void DrawAxis(float Size = 1.0f, const CColor &Color = colWHITE) { DrawAxis(Size, Size, Size, Color); }
	void DrawGrid(Vect3f Size, const CColor &Color = colWHITE) { DrawGrid(Size.x, Size.y, Size.z, Color); }
	void DrawGrid(float Size = 1.0f, const CColor &Color = colWHITE) { DrawGrid(Size, Size, Size, Color); }
	void DrawCube(Vect3f Size, const CColor &Color = colWHITE) { DrawCube(Size.x, Size.y, Size.z, Color); }
	void DrawCube(float Size = 1.0f, const CColor &Color = colWHITE) { DrawCube(Size, Size, Size, Color); }

	void Draw_Triangle();
	void CreateDebugObjects();

	// Matrix Operations
	void SetModelMatrix(const Mat44f &Model);
	void SetViewMatrix(const Mat44f &View);
	void SetProjectionMatrix(const Mat44f &Projection);
	void SetViewProjectionMatrix(const Mat44f &View, const Mat44f &Projection);
	void SetViewMatrix(const Vect3f& vPos, const Vect3f& vTarget, const Vect3f& vUp);
	void SetProjectionMatrix(float fovy, float aspect, float zn, float zf);

	ID3D11Device* GetDevice() { return m_Device.get();}
	ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext.get();}

};

#endif //_ENGINE_RENDERMANAGER_CPB_2016110320428_H
