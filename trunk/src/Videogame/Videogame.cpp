#include "Utils/MemLeaks/MemLeaks.h"
#include "Engine\Materials\MaterialManager.h"
#include "Engine\Render\RenderManager.h"
#include "Engine\Input\InputManager.h"
#include "Engine\Input\ActionManager.h"
#include "Engine\imgui_impl_dx11.h"
#include "Engine\SphericalCameraController.h"
#include "Engine\FpsCameraController.h"
#include "Engine\TpsCameraController.h"
#include "ImGUI\imgui.h"
#include "Engine\Engine.h"
#include <Windows.h>
#include <chrono>

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
	CRenderManager l_RenderManager;
	l_RenderManager.SetModelMatrix(m_model_matrix);
	l_RenderManager.SetProjectionMatrix(45.0f, m_width / m_height, 0.5f, 100.0f);
	l_RenderManager.SetViewMatrix(m_vpos, m_vtarget, m_vup);
	l_RenderManager.Init(hWnd, 800, 600);

	
	CInputManager l_InputManager(hWnd);
	CActionManager l_ActionManager(l_InputManager);

	std::string inputConfigPath = "data/config/input_config.xml";
	l_ActionManager.LoadActions(inputConfigPath);

	// Setup iMGui binding
	ImGui_ImplDX11_Init(hWnd, l_RenderManager.GetDevice(), l_RenderManager.GetDeviceContext());

	// Setup Cameras
	CSphericalCameraController l_SphericalCamera(Vect3f(0, 0, 0), 1.5f, -1.5f, 20.0f, 1.0f);
	CFpsCameraController l_FpsCamera(Vect3f(0, 1.0, 0), 1.5f, -1.5f);
	CTpsCameraController l_TpsCamera(Vect3f(0, 0, 0), 1.5f, -1.5f, 20.0f, 4.0f);

	CEngine l_Engine = CEngine();
	l_Engine.SetActionManager(&l_ActionManager);
	l_Engine.SetCameraController(&l_SphericalCamera);
	l_Engine.SetRenderManager(&l_RenderManager);
	l_Engine.fpsCam = l_FpsCamera;
	l_Engine.orbitalCam = l_SphericalCamera;
	l_Engine.tpsCam = l_TpsCamera;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	l_RenderManager.SetProjectionMatrix(0.8f, 800.0f / 600.0f, 0.5f, 100.5f);
	
	int l_prevCameraSelector = 0;
	std::chrono::monotonic_clock l_Clock;
	std::chrono::monotonic_clock::time_point l_PrevTime;
	float timer = 0.0f;
	std::string s_fps("");
	float fps = 0.0f;

	while (msg.message != WM_QUIT) 	{

		auto currentTime = l_Clock.now();
		std::chrono::duration<float> chronoDeltaTime = currentTime - l_PrevTime;
		l_PrevTime = currentTime;

		float dt = chronoDeltaTime.count() > 0.5f ? 0.5f : chronoDeltaTime.count();

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
		l_Engine.ProcessInputs();
		l_Engine.Update();
		l_Engine.Render();

		

	
	}

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	#ifdef _DEBUG
		MemLeaks::MemoryEnd();
	#endif	

	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	return 0;
}
