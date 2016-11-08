#include "Utils/MemLeaks/MemLeaks.h"
#include "Engine\Materials\MaterialManager.h"
#include "Engine\Render\RenderManager.h"
#include <Windows.h>

#define APPLICATION_NAME	"ESCAPE FROM ALCATRAZ"

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

	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	// Añadir aquí el Init de la applicacioón

	engine::render::CRenderManager l_RenderManager;

	l_RenderManager.Init(hWnd, 800, 600);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	//l_RenderManager.SetProjectionMatrix(0.8f, 800.0f / 600.0f, 0.5f, 100.5f);

	//std::chrono::high_resolution_clock clock;

	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			bool fHandled = false;

			if (!fHandled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
				break;
		}

	{
		l_RenderManager.BeginRender();

		l_RenderManager.DoRender();

		l_RenderManager.EndRender();
	}
	}
	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	return 0;


  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

#ifdef _DEBUG
  MemLeaks::MemoryEnd();
#endif	


  return 0;
}
