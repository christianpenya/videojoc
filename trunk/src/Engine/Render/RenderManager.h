#ifndef _ENGINE_RENDERMANAGER_CPB_2016110320428_H
#define _ENGINE_RENDERMANAGER_CPB_2016110320428_H

#include <Windows.h>
#include <winerror.h>
#include "d3dcompiler.h"
#include "d3d11.h"
#include "dxgi.h"
#include "Math/Color.h"

#ifdef _DEBUG
#include <assert.h>
#endif

namespace engine {
	namespace render {

		// Render Manager Class
		class CRenderManager {
		
		public:
			struct CDebugVertex {
				Vect4f Position;
				CColor Color;
			};

			static const int DebugVertexBufferSize = 0x10000;

		protected:
			ID3D11Device							*m_Device;
			ID3D11DeviceContext						*m_DeviceContext;
			IDXGISwapChain							*m_SwapChain;
			ID3D11RenderTargetView					*m_RenderTargetView;
			ID3D11Texture2D							*m_DepthStencil;
			ID3D11DepthStencilView					*m_DepthStencilView;

			ID3D11Buffer							*m_DebugVertexBuffer;
			int										m_DebugVertexBufferCurrentIndex;
			ID3D11VertexShader						*m_DebugVertexShader;
			ID3D11InputLayout						*m_DebugVertexLayout;
			ID3D11PixelShader						*m_DebugPixelShader;

			const CDebugVertex*						m_AxisRenderableVertexs;
			const CDebugVertex*						m_GridRenderableVertexs;
			const CDebugVertex*						m_CubeRenderableVertexs;
			const CDebugVertex*						m_SphereRenderableVertexs;

			int m_NumVerticesAxis, m_NumVerticesGrid, m_NumVerticesCube, m_NumVerticesSphere;

			ID3D11RasterizerState					*m_SolidRenderState;

			//Mat44f m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix;
			//Mat44f m_ViewProjectionMatrix, m_ModelViewProjectionMatrix;

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
			void Draw_Triangle();
			void CreateDebugObjects();
		};
	}
}

#endif //_ENGINE_RENDERMANAGER_CPB_2016110320428_H