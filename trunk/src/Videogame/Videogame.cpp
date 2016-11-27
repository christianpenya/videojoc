#include "Utils/MemLeaks/MemLeaks.h"
#include "Engine\Materials\MaterialManager.h"
#include "Engine\Render\RenderManager.h"
#include "Engine\Input\InputManager.h"
#include "Engine\Input\ActionManager.h"
#include "Engine\imgui_impl_dx11.h"
#include "Engine\SphericalCameraController.h"
#include "Engine\FpsCameraController.h"
#include "Engine\TpsCameraController.h"
#include "Base\ImGUI\imgui.h"
#include <Windows.h>

#define APPLICATION_NAME	"ESCAPE FROM ALCATRAZ"

const Mat44f m_model_matrix = Mat44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const Vect3f m_vpos = Vect3f(10, 5, 7);
const Vect3f m_vtarget = Vect3f(0, 0, 0);
const Vect3f m_vup = Vect3f(0, 1, 0);

const int m_width = 800;
const int m_height = 600;

int m_prevCameraSelector = 0;

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

CRenderManager initRenderManager(HWND& hWnd) {

	CRenderManager l_RenderManager;

	l_RenderManager.SetModelMatrix(m_model_matrix);
	l_RenderManager.SetProjectionMatrix(45.0f, m_width / m_height, 0.5f, 100.0f);
	l_RenderManager.SetViewMatrix(m_vpos, m_vtarget, m_vup);

	l_RenderManager.Init(hWnd, 800, 600);

	return l_RenderManager;
}

float clamp(float x, float upper, float lower)
{
	return min(upper, max(x, lower));
}


void sphere(CRenderManager& renderManager, CActionManager& actionManager, Vect3f front = Vect3f(0,0,1), Vect3f up = Vect3f(0,1,0)) {

	Vect3f right = front ^ up;
	float smoother = 0.1f;

	renderManager.m_SphereOffset += actionManager("z_move")->value * front * smoother;
	renderManager.m_SphereOffset += actionManager("x_move")->value * right * smoother;
	
	renderManager.m_SphereOffset.x = clamp(renderManager.m_SphereOffset.x, 5.0, -5.0);
	renderManager.m_SphereOffset.y = 1;
	renderManager.m_SphereOffset.z = clamp(renderManager.m_SphereOffset.z, 5.0, -5.0);

	renderManager.DrawSphere(1, CColor(1, 1, 1, 1));
}

void orbitalCamera(CSphericalCameraController& sphericalCamera, CActionManager& actionManager) {
	
	ImGui::Text("Gira la roda del mouse per controlar el zoom.");
	ImGui::Text("Mantingues apretada la roda del mouse per rotar la camera.");

	sphericalCamera.zoomSpeed = actionManager("zoom")->value;
	
	if (actionManager("enable_orientation")->active) {
		sphericalCamera.yawSpeed = 0.1 * actionManager("vertical_orientation")->value;
		sphericalCamera.pitchSpeed = -0.1 * actionManager("horizontal_orientation")->value;
	}
	else {
		sphericalCamera.yawSpeed = 0.0;
		sphericalCamera.pitchSpeed = 0.0;
	}

	sphericalCamera.Update(0.016f);
}

void fpsCamera(CFpsCameraController& fpsCamera, CActionManager& actionManager) {

	ImGui::Text("Mou la camera amb WASD.");
	ImGui::Text("Orienta't amb el ratoli.");
	ImGui::Text("Vigila el cap.");

	fpsCamera.xSpeed = 0.1f * actionManager("x_move")->value;
	fpsCamera.zSpeed = 0.1f * actionManager("z_move")->value;
	
	fpsCamera.yawSpeed = 0.1 * actionManager("vertical_orientation")->value;
	fpsCamera.pitchSpeed = -0.1 * actionManager("horizontal_orientation")->value;
	
	fpsCamera.Update(0.016f);
}

void TpsCamera(CTpsCameraController& tpsCamera, CActionManager& actionManager, Vect3f sphereCenter) {

	ImGui::Text("Mou l'esfera amb WASD");
	ImGui::Text("Orienta't amb el ratoli.");
	ImGui::Text("Amb la roda del ratoli pots controlar el zoom.");

	tpsCamera.center = sphereCenter;

	tpsCamera.yawSpeed = 0.1 * actionManager("vertical_orientation")->value;
	tpsCamera.pitchSpeed = -0.1 * actionManager("horizontal_orientation")->value;

	tpsCamera.zoomSpeed = actionManager("zoom")->value;

	tpsCamera.Update(0.016f);
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
	CRenderManager l_RenderManager = initRenderManager(hWnd);
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

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	l_RenderManager.SetProjectionMatrix(0.8f, 800.0f / 600.0f, 0.5f, 100.5f);
	//std::chrono::high_resolution_clock clock;

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

		static bool show_app_auto_resize = false;
		ImGui::Begin("Motor3D", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);
		
		static int cameraSelector = 0;
		
		ImGui::RadioButton("Orbital", &cameraSelector, 0); ImGui::SameLine();
		ImGui::RadioButton("FPS", &cameraSelector, 1); ImGui::SameLine();
		ImGui::RadioButton("TPS", &cameraSelector, 2);
		
		l_RenderManager.DrawGrid(1, 1, 1, CColor(0, 0, 0, 1));

		// Reiniciem posició de l'esfera quan canviem de camera
		if (cameraSelector != m_prevCameraSelector) {
			l_RenderManager.m_SphereOffset = Vect3f(0, 0, 0);
		}

		m_prevCameraSelector = cameraSelector;

		switch (cameraSelector) {
		case 0: //Orbital
			l_RenderManager.DrawSphere(1, CColor(1, 1, 0, 1));
			orbitalCamera(l_SphericalCamera, l_ActionManager);
			l_SphericalCamera.SetToRenderManager(l_RenderManager);
			break;
		case 1: //FPS
			l_RenderManager.m_SphereOffset = Vect3f(0, 3, 0);
			l_RenderManager.DrawSphere(1, CColor(0, 1, 1, 1));
			fpsCamera(l_FpsCamera, l_ActionManager);
			l_FpsCamera.SetToRenderManager(l_RenderManager);
			break;
		case 2: //TPS
			sphere(l_RenderManager, l_ActionManager, l_TpsCamera.getFront(), l_TpsCamera.getUp());
			TpsCamera(l_TpsCamera, l_ActionManager, l_RenderManager.m_SphereOffset);
			l_TpsCamera.SetToRenderManager(l_RenderManager);
			break;
		default:
			break;
		}

		ImGui::End();
		ImGui::Render();
		l_RenderManager.CreateDebugObjects();
		l_RenderManager.EndRender();
	}

	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	#ifdef _DEBUG
		MemLeaks::MemoryEnd();
	#endif	

	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	return 0;
}
