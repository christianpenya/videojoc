#include "SceneNode.h"

#include "Render/RenderManager.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "ImGUI/imgui.h"
#include "SceneGUI.h"
#include "Engine/Engine.h"
#include"GUI/GUIManager.h"
#include "Render/RenderManager.h"
#include "GUI/GUIPosition.h"
#include "Events/LevelController.h"

CSceneGUI::CSceneGUI(const CXMLElement* TreeNode)
    : CSceneNode(TreeNode),
      nodeName(TreeNode->GetAttribute<std::string>("name", ""))
{
    gui_element = TreeNode->GetAttribute<std::string>("gui_element", "");
    portion = TreeNode->GetAttribute<Vect2f>("portion", Vect2f(0, 0));
    size = TreeNode->GetAttribute<Vect2f>("size", Vect2f(0, 0));
    type = TreeNode->GetAttribute<std::string>("type", "");
    action = TreeNode->GetAttribute<int>("action", 0);
}

CSceneGUI::~CSceneGUI() {}

bool CSceneGUI::Render(CRenderManager &RenderManager)
{

    return true;
}
bool CSceneGUI::Update(float ElapsedTime)
{
    CGUIManager *guiMan = &CEngine::GetInstance().GetGUIManager();
    CRenderManager *renderMan = &CEngine::GetInstance().GetRenderManager();
    Vect2u l_size = renderMan->GetWindowSize();
    if (strcmp(type.c_str(), "GUIsprite") == 0)
    {
        guiMan->DoGUISprite(nodeName, gui_element, CGUIPosition(l_size.x*portion.x, l_size.y*portion.y, size.x, size.y));
    }
    else if (strcmp(type.c_str(), "button") == 0)
    {
        switch (action)
        {
        case 1://Reanuda el juego
        {

            if (guiMan->DoButton(nodeName, gui_element, CGUIPosition(l_size.x*portion.x, l_size.y*portion.y, size.x, size.y)))
            {
                CLevelController *contr = CEngine::GetInstance().m_LevelController;
                if (contr)
                    contr->ResumeGame();
            }

        }
        break;
        case 2://RestoreLastCheckpoint
            if (guiMan->DoButton(nodeName, gui_element, CGUIPosition(l_size.x*portion.x, l_size.y*portion.y, size.x, size.y)))
            {
                CLevelController *contr = CEngine::GetInstance().m_LevelController;
                if (contr)
                {
                    contr->RestoreLastCheckpoint();
                    contr->ResumeGame();
                }
            }

            break;
        default:
        case 3:
        {
            if (guiMan->DoButton(nodeName, gui_element, CGUIPosition(l_size.x*portion.x, l_size.y*portion.y, size.x, size.y)))
            {

                CLevelController *contr = CEngine::GetInstance().m_LevelController;
                if (contr)
                {
                    contr->NewGame();
                }

            }
        }
        case 4:
        {
            if (guiMan->DoButton(nodeName, gui_element, CGUIPosition(l_size.x*portion.x, l_size.y*portion.y, size.x, size.y)))
            {

                CLevelController *contr = CEngine::GetInstance().m_LevelController;
                if (contr)
                {
                    contr->ToMainMenu();
                }

            }
        }
        break;
        }

    }
    return true;
}


