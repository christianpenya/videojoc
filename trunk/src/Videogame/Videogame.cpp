#include "Utils/MemLeaks/MemLeaks.h"
#include "Materials\MaterialManager.h"
#include "Textures\TextureManager.h"
#include "Render\RenderManager.h"
#include "RenderPipeline\RenderPipeline.h"
#include "Input\InputManager.h"
#include "Input\ActionManager.h"
#include "Mesh\MeshManager.h"
#include "Effects\ShaderManager.h"
#include "Effects\EffectManager.h"
#include "Effects\TechniquePoolManager.h"
#include "Scenes\SceneManager.h"
#include "Engine\imgui_impl_dx11.h"
#include "ImGUI\imgui.h"
#include "Engine\Engine.h"
#include "Lights\LightManager.h"
#include <Windows.h>
#include <chrono>
#include <memory>


#define APPLICATION_NAME	"ESCAPE FROM ALCATRAZ"

const Mat44f m_model_matrix = Mat44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
const Vect3f m_vpos = Vect3f(10, 5, 7);
const Vect3f m_vtarget = Vect3f(0, 0, 0);
const Vect3f m_vup = Vect3f(0, 1, 0);

float m_width = 1600;
float m_height = 900;

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
            if (CEngine::GetInstance().HasRenderManager())
            {
                auto& renderManager = CEngine::GetInstance().GetRenderManager();
                renderManager.Resize(LOWORD(lParam), HIWORD(lParam), hWnd);
                //CEngine::GetInstance().Init(hWnd);
            }
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
    _hInstance = _hInstance;
    _hPrevInstance = _hPrevInstance;
    _lpCmdLine = _lpCmdLine;
    _nCmdShow = _nCmdShow;

#ifdef _DEBUG
    MemLeaks::MemoryBegin();
    debug=true;
#endif

    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

    RegisterClassEx(&wc);

    RECT rc = { 0, 0, (long)m_width, (long)m_height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the application's window
    HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

    // Añadir aquí el Init de la applicación

    CRenderManager l_RenderManager;
    l_RenderManager.SetModelMatrix(m_model_matrix);
    l_RenderManager.SetProjectionMatrix(45.0f, m_width / m_height, 0.5f, 100.0f);
    l_RenderManager.SetViewMatrix(m_vpos, m_vtarget, m_vup);
    l_RenderManager.Init(hWnd, (int)m_width, (int)m_height, debug);

    //CRenderPipeline l_RenderManagerP;
    //l_RenderManagerP.Load("data/render_pipeline.xml");


    CInputManager l_InputManager(hWnd);
    CActionManager l_ActionManager(l_InputManager);

    CMaterialManager l_MaterialManager;

    std::string inputConfigPath = "data/config/input_config.xml";
    l_ActionManager.LoadActions(inputConfigPath);

    // Setup iMGui binding
    ImGui_ImplDX11_Init(hWnd, l_RenderManager.GetDevice(), l_RenderManager.GetDeviceContext());

    // Setup Cameras
    CSphericalCameraController l_SphericalCamera(Vect3f(0, 0, 0), 1.5f, -1.5f, 20.0f, 1.0f);
    CFpsCameraController l_FpsCamera(Vect3f(0, 1.0, 0), 1.5f, -1.5f);
    CTpsCameraController l_TpsCamera(Vect3f(0, 0, 0), 1.5f, -1.5f, 20.0f, 4.0f);

    CEngine& l_Engine = CEngine::GetInstance();
    l_Engine.SetActionManager(&l_ActionManager);
    l_Engine.SetCameraController(&l_SphericalCamera);
    l_Engine.SetRenderManager(&l_RenderManager);
    l_Engine.SetMaterialManager(&l_MaterialManager);
    l_Engine.fpsCam = l_FpsCamera;
    l_Engine.orbitalCam = l_SphericalCamera;
    l_Engine.tpsCam = l_TpsCamera;

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    std::chrono::monotonic_clock l_Clock;
    std::chrono::monotonic_clock::time_point l_PrevTime;
    std::string s_fps("");

    std::string lLevelMaterialsFilename = "reclusion.xml";
    std::string lDefaultMaterialsFilename = "default.xml";
    /*
    l_MaterialManager.Load(lLevelMaterialsFilename, lDefaultMaterialsFilename);

    CTextureManager* l_TextureManager = new CTextureManager();

        CMeshManager* l_MeshManager = new CMeshManager();
        l_MeshManager->GetMesh("data/meshes/Plane001.mesh");
        //l_MeshManager->GetMesh("data/meshes/Plane002.mesh");
        l_Engine.SetMeshManager(l_MeshManager);

    CShaderManager* l_ShaderManager = new CShaderManager();
    l_ShaderManager->Load("data/shaders.xml");
    l_Engine.SetShaderManager(l_ShaderManager);

    CEffectManager* l_EffectManager = new CEffectManager();
    l_EffectManager->Load("data/effects.xml");
    l_Engine.SetEffectManager(l_EffectManager);

    CTechniquePoolManager* l_TechniquePoolManager = new CTechniquePoolManager();
    l_TechniquePoolManager->Load("data/techniques_pool.xml");
    l_TechniquePoolManager->Apply("forward");
    l_Engine.SetTechniquePoolManager(l_TechniquePoolManager);

    CLightManager* l_LightManager = new CLightManager();
    l_LightManager->Load("data/lights.xml");
    l_Engine.SetLightManager(l_LightManager);
    */

    //CSceneManager* l_SceneManager = new CSceneManager();
    //l_SceneManager->Load("data/scenes.xml");

    while (msg.message != WM_QUIT)
    {
        auto currentTime = l_Clock.now();
        std::chrono::duration<float> chronoDeltaTime = currentTime - l_PrevTime;
        l_PrevTime = currentTime;


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
                break;
            }
        }

        l_InputManager.PostUpdate();
        l_Engine.ProcessInputs();
        l_Engine.Update();
        l_Engine.Render();
    }

    // TODO Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
#ifdef _DEBUG
    l_RenderManager.ReportLive();
    MemLeaks::MemoryEnd();
#endif

    UnregisterClass(APPLICATION_NAME, wc.hInstance);
    return 0;
}
