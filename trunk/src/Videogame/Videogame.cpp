#include "Utils/MemLeaks/MemLeaks.h"
#include "Engine\Materials\MaterialManager.h"
#include "Engine\Render\RenderManager.h"
#include "Engine\Input\InputManager.h"
#include "Engine\Input\ActionManager.h"
#include "Engine\imgui_impl_dx11.h"
#include "Engine\SphericalCameraController.h"
#include "Base\ImGUI\imgui.h"
#include <Windows.h>

#define APPLICATION_NAME	"ESCAPE FROM ALCATRAZ"

const Mat44f m_model_matrix = Mat44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const Vect3f m_vpos = Vect3f(10, 5, 7);
const Vect3f m_vtarget = Vect3f(0, 0, 0);
const Vect3f m_vup = Vect3f(0, 1, 0);

const int m_width = 800;
const int m_height = 600;

bool s_WindowActive;

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
  case WM_ACTIVATE:
  {
	s_WindowActive = wParam != WA_INACTIVE;
	  break;
  }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

engine::render::CRenderManager initRenderManager(HWND& hWnd) {

	engine::render::CRenderManager l_RenderManager;

	l_RenderManager.SetModelMatrix(m_model_matrix);
	l_RenderManager.SetProjectionMatrix(45.0f, m_width / m_height, 0.5f, 100.0f);
	l_RenderManager.SetViewMatrix(m_vpos, m_vtarget, m_vup);

	l_RenderManager.Init(hWnd, 800, 600);

	return l_RenderManager;
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{

  #ifdef _DEBUG
	MemLeaks::MemoryBegin();
  #endif

	//engine::materials::CMaterialManager manager = engine::materials::CMaterialManager();
	//manager.LoadXML();
	
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx(&wc);

	RECT rc = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	// Añadir aquí el Init de la applicacioón
	engine::render::CRenderManager l_RenderManager = initRenderManager(hWnd);
	engine::input::CInputManager l_InputManager(hWnd);
	engine::input::CActionManager l_ActionManager(l_InputManager);

	std::string inputConfigPath = "data/config/input_config.xml";
	l_ActionManager.LoadActions(inputConfigPath);

	// Setup iMGui binding
	ImGui_ImplDX11_Init(hWnd, l_RenderManager.GetDevice(), l_RenderManager.GetDeviceContext());

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	//l_RenderManager.SetProjectionMatrix(0.8f, 800.0f / 600.0f, 0.5f, 100.5f);

	//std::chrono::high_resolution_clock clock;

	SphericalCameraController l_sphericalCamera (Vect3f(0,0,0) ,1.5f, -1.5f, 20.0f, 1.0f);

	while (msg.message != WM_QUIT) 	{
		
		l_InputManager.PreUpdate(s_WindowActive);

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			
			ImGui_ImplDX11_WndProcHandler(hWnd, msg.message, msg.wParam, msg.lParam);
			bool fHandled = false;

			if ((msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) || msg.message == WM_INPUT) {
				fHandled = l_InputManager.HandleMouse(msg);
			}
			else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) {
				fHandled = l_InputManager.HandleKeyboard(msg);
			}

			if (!fHandled) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT) {
				break;
			}			
		}

		l_InputManager.PostUpdate();
		l_ActionManager.Update();
		ImGui_ImplDX11_NewFrame();

		l_RenderManager.BeginRender();
		
		//ImGui::Begin("Test Window");
		//ImGui::Text("Ola k ase world");
		//ImGui::End();

		l_RenderManager.DrawGrid(1, 1, 1, CColor(1, 1, 1, 1));

		//if (l_ActionManager("vertical")->active) {
		//	l_sphericalCamera.zoomSpeed = -1.0 * l_ActionManager("vertical")->value;
		//}
		//else {
		//	l_sphericalCamera.zoomSpeed = 0.0f;
		//}

		l_sphericalCamera.zoomSpeed = l_InputManager.GetMouseMovementZ();

		if (l_ActionManager("enable_orientation")->active) {
				l_sphericalCamera.yawSpeed = 0.1 * l_ActionManager("vertical_orientation")->value;
				l_sphericalCamera.pitchSpeed = -0.1 * l_ActionManager("horizontal_orientation")->value;
		}
		else {
			l_sphericalCamera.yawSpeed = 0.0;
			l_sphericalCamera.pitchSpeed = 0.0;
		}
	
		l_sphericalCamera.Update(0.016f);
		l_sphericalCamera.SetToRenderManager(l_RenderManager);

		ImGui::Render();
		l_RenderManager.EndRender();
	}

	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	return 0;
	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

#ifdef _DEBUG
  MemLeaks::MemoryEnd();
#endif	

  return 0;
}


