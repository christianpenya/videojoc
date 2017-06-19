#include "Utils/MemLeaks/MemLeaks.h"
#include "Render\RenderManager.h"
#include "Input\InputManager.h"
#include "Input\ActionManager.h"
#include "imGUI/imgui_impl_dx11.h"
#include "ImGUI\imgui.h"
#include "Engine\Engine.h"
#include <Windows.h>
#include "Utils/Logger.h"

#define APPLICATION_NAME	"ESCAPE FROM ALCATRAZ"

float m_width = 1024;
float m_height = 768;

const Mat44f m_model_matrix = Mat44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const Vect3f m_vpos = Vect3f(10, 5, 7);
const Vect3f m_vtarget = Vect3f(0, 0, 0);
const Vect3f m_vup = Vect3f(0, 1, 0);

bool s_WindowActive;
bool debug=false;
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
    case WM_SIZE:
    {
        if (wParam != SIZE_MINIMIZED)
        {
            if (CEngine::GetInstance().ExistRenderManager())
            {
                auto& renderManager = CEngine::GetInstance().GetRenderManager();
                renderManager.Resize(LOWORD(lParam), HIWORD(lParam), hWnd);
            }
        }
    }
    break;
    case WM_CHAR:
    {
        if (CEngine::GetInstance().ExistInputManager())
        {
            CEngine::GetInstance().GetInputManager().SetLastCharInput(wParam);
        }
    }

    }//end switch( msg )

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int main()
{

#ifdef _DEBUG
    MemLeaks::MemoryBegin();
    debug=true;
#endif

    bool lQuit = false;

    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

    RegisterClassEx(&wc);

    RECT rc = { 0, 0, (long)m_width, (long)m_height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the application's window
    HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);


    //RENDER MANAGER
    CRenderManager l_RenderManager;
    l_RenderManager.SetModelMatrix(m_model_matrix);
    l_RenderManager.SetProjectionMatrix(45.0f, m_width / m_height, 0.5f, 100.0f);
    l_RenderManager.SetViewMatrix(m_vpos, m_vtarget, m_vup);
    l_RenderManager.Init(hWnd, (int)m_width, (int)m_height, debug);

    CEngine& l_Engine = CEngine::GetInstance();
    l_Engine.SetRenderManager(&l_RenderManager);
    l_Engine.Init(hWnd);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    CInputManager& l_InputManager = CEngine::GetInstance().GetInputManager();

    while (msg.message != WM_QUIT)
    {

        l_InputManager.PreUpdate(s_WindowActive);

        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {

            ImGui_ImplDX11_WndProcHandler(hWnd, msg.message, msg.wParam, msg.lParam);
            bool fHandled = false;

            if ((msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) || msg.message == WM_INPUT)
            {
                fHandled = l_InputManager.HandleMouse(msg);
            }
            else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
            {
                fHandled = l_InputManager.HandleKeyboard(msg);
                bool WasDown = ((msg.lParam & (1 << 30)) != 0);
                bool IsDown = ((msg.lParam & (1 << 31)) == 0);
                bool Alt = ((msg.lParam & (1 << 29)) != 0);

                if (!WasDown && IsDown && Alt && msg.wParam == VK_RETURN)
                {
                    WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
                    GetWindowPlacement(msg.hwnd, &windowPosition);

                    l_RenderManager.ToggleFullscreen(msg.hwnd, windowPosition);
                    fHandled = true;
                }
                else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                {
                    PostQuitMessage(0);
                }
                else
                {
                    fHandled = l_InputManager.HandleKeyboard(msg);
                }
            }

            if (!fHandled)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (msg.message == WM_QUIT)
            {
                lQuit = true;
                break;
            }
        }

        if (!lQuit)
        {
            l_InputManager.PostUpdate();
            l_Engine.ProcessInputs();
            l_Engine.Update();
            l_Engine.Render();
        }
    }

    // TODO Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
#ifdef _DEBUG
    l_RenderManager.ReportLive();
    MemLeaks::MemoryEnd();
#endif

    UnregisterClass(APPLICATION_NAME, wc.hInstance);
    return 0;
}
