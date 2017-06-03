#include "GUIManager.h"
#include "Graphics/Materials/Material.h"
#include "Button.h"
#include "GUIPosition.h"
#include "Slider.h"
#include "Render/RenderManager.h"
#include "Graphics/Effects/EffectManager.h"
#include <assert.h>
#include "Input/InputManager.h"
#include "XML/XML.h"
#include "Graphics/Mesh/TemplatedGeometry.h"


CGUIManager::CGUIManager()
{
    m_ActiveItem = "";
    m_HotItem = "";
    m_InputUpToDate = false;
    m_MouseWentPressed = false;
    m_MouseWentReleased = false;
}

CGUIManager::~CGUIManager()
{
    Destroy();
}

void CGUIManager::Destroy()
{
    for (size_t i = 0; i < m_Materials.size(); i++)
        base::utils::CheckedDelete(m_Materials[i]);
    m_Materials.clear();

    /*for (size_t i = 0; i < m_VertexBuffers.size(); i++)
        base::utils::CheckedDelete(m_VertexBuffers[i]);*/
    m_VertexBuffers.clear();

    for (auto it = m_Buttons.begin(); it != m_Buttons.end(); it++)
        base::utils::CheckedDelete(it->second);
    m_Buttons.clear();
    /*
    for (auto it = m_Sliders.begin(); it != m_Sliders.end(); it++)
        base::utils::CheckedDelete(it->second);
    m_Sliders.clear();
    */

    m_SpriteMaps.clear();
    m_Sprites.clear();
}

void CGUIManager::SetActive(const std::string& id)
{
    m_ActiveItem = id;
    m_SelectedItem = "";
}

void CGUIManager::SetNotActive()
{
    m_ActiveItem = "";
}

void CGUIManager::SetHot(const std::string& id)
{
    if (m_ActiveItem == "" || m_ActiveItem == id)
        m_HotItem = id;
}

void CGUIManager::SetNotHot(const std::string& id)
{
    if (m_HotItem == id)
        m_HotItem = "";
}

void CGUIManager::SetSelected(const std::string& id)
{
    m_SelectedItem = id;
}

void CGUIManager::SetNotSelected(const std::string& id)
{
    if (m_SelectedItem == id)
        m_SelectedItem = "";
}

bool CGUIManager::Load(std::string _FileName)
{
    CRenderManager &aRM = CEngine::GetInstance().GetRenderManager();

    m_FileName = _FileName;
    CXMLDocument document;
    EXMLParseError error = document.LoadFile(m_FileName.c_str());
    SpriteMapInfo l_spriteMapinfo;
    float u1, u2, v1, v2, w, h;
    SpriteInfo l_sprite;
    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lGUI_Elements = document.FirstChildElement("gui_elements");
        if (lGUI_Elements)
        {

            for (tinyxml2::XMLElement *iElement = lGUI_Elements->FirstChildElement(); iElement != nullptr; iElement = iElement->NextSiblingElement())
            {
                if (strcmp(iElement->Name(), "gui_spritemap") == 0)
                {
                    std::string l_spriteMapName = iElement->GetAttribute<std::string>("name", "");

                    l_spriteMapinfo = { m_Materials.size(), iElement->GetAttribute("width",0), iElement->GetAttribute("height",0) };

                    m_SpriteMaps[l_spriteMapName] = l_spriteMapinfo;
                    for (tinyxml2::XMLElement *iSubElement = iElement->FirstChildElement(); iSubElement != nullptr; iSubElement = iSubElement->NextSiblingElement())
                    {
                        if (strcmp(iSubElement->Name(), "material"))
                        {
                            m_Materials.push_back(new CMaterial(iSubElement));
                            m_VertexBuffers.push_back( CGeometryTriangleList<VertexTypes::SpriteVertex>(
                                                           new CVertexBuffer<VertexTypes::SpriteVertex>(aRM, nullptr, MAX_VERTICES_PER_CALL)
                                                       )
                                                     );
                        }
                        else if (strcmp(iSubElement->Name(), "sprite"))
                        {
                            w = iSubElement->GetAttribute<float>("w",0.0f);
                            h = iSubElement->GetAttribute<float>("h", 0.0f);
                            u1 = iSubElement->GetAttribute<float>("x", 0.0f);
                            v1 = iSubElement->GetAttribute<float>("y", 0.0f);
                            u2 = u1 + w;
                            v2 = v1 + h;
                            l_sprite = { &m_SpriteMaps[l_spriteMapName], u1 / l_spriteMapinfo.w, u2 / l_spriteMapinfo.w, v1 / l_spriteMapinfo.h, v2 / l_spriteMapinfo.h };
                            m_Sprites[iSubElement->GetAttribute<std::string>("name", "")] = l_sprite;
                        }
                    }


                }
                else if (strcmp(iElement->Name(), "button") == 0)
                {
                    //TODO
                    m_Buttons[iElement->GetAttribute<std::string>("name", "")] = new CButon(&m_Sprites[iElement->GetAttribute<std::string>("normal", "")], &m_Sprites[iElement->GetAttribute<std::string>("highlight", "")], &m_Sprites[iElement->GetAttribute<std::string>("pressed", "")]);

                }

                else if (strcmp(iElement->Name(), "slider") == 0)
                {
                    //TODO
                    //m_Sliders[iElement->GetAttribute<std::string>("name", "")] = new Slider();
                }

            }

        }
    }
    else
    {
        return false;
    }
    return true;
}

bool IsMouseInside(int _mouseX, int _mouseY, int x, int y, int width, int height)
{
    if ((_mouseX >= x && _mouseX <= x + width) && (_mouseY >= y && _mouseY <= y + height))
        return true;
    return false;
}

void CGUIManager::CheckInput()
{
    if (!m_InputUpToDate)
    {
        CInputManager inputMan = CEngine::GetInstance().GetInputManager();
        m_MouseX = inputMan.GetMouseX();
        m_MouseY = inputMan.GetMouseY();

        m_MouseWentPressed = inputMan.LeftMouseButtonBecomesPressed();
        m_MouseWentReleased = inputMan.LeftMouseButtonBecomesReleased();

        //TODO
        m_InputUpToDate = true;
    }
}

void CGUIManager::Render(CRenderManager *RenderManager)
{

    VertexTypes::SpriteVertex m_CurrentBufferData[MAX_VERTICES_PER_CALL];
    int currentVertex = 0;
    SpriteMapInfo *currentSpriteMap = nullptr;
    for (size_t i = 0; i < m_PanelCommands.size(); ++i)
    {
        GUICommand &command = m_PanelCommands[i];
        assert(command.x1 <= command.x2);
        assert(command.y2 <= command.y2);

        SpriteInfo *commandSprite = command.sprite;
        SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

        if (currentSpriteMap != commandSpriteMap || currentVertex == 5)
        {
            if (currentVertex > 0)
            {
                //TODO log a warning if we get here by "currentVertex == s_MaxVerticesPerCall"
                //TODO draw all current vertex in the currentBuffer

                m_Materials[currentSpriteMap->MaterialIndex]->Apply();
                m_VertexBuffers[currentSpriteMap->MaterialIndex].UpdateVertex(m_CurrentBufferData, currentVertex);
                m_VertexBuffers[currentSpriteMap->MaterialIndex].Render(RenderManager->GetDeviceContext());
            }
            currentVertex = 0;
            currentSpriteMap = commandSpriteMap;
        }
        int l_Height = RenderManager->GetWindowSize().y;
        int l_Width = RenderManager->GetWindowSize().x;
        float x1 = (command.x1 / (l_Width * 0.5f)) - 1.0f;
        float x2 = (command.x2 / (l_Width * 0.5f)) - 1.0f;
        float y1 = 1.0f - (command.y1 / (l_Height * 0.5f));
        float y2 = 1.0f - (command.y2 / (l_Height * 0.5f));

        float u1 = commandSprite->u1 * (1.0f - command.u1) + commandSprite->u2 * command.u1;
        float u2 = commandSprite->u1 * (1.0f - command.u2) + commandSprite->u2 * command.u2;
        float v1 = commandSprite->v1 * (1.0f - command.v1) + commandSprite->v2 * command.v1;
        float v2 = commandSprite->v1 * (1.0f - command.v2) + commandSprite->v2 * command.v2;

        //assert(MAX_VERTICES_PER_CALL > currentVertex + 6);

        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y2, 0.f), Vect2f(u1, v2), command.color }; // { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1), command.color };

        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y1, 0.f), Vect2f(u2, v1), command.color };
    }

    for (size_t i = 0; i < m_Commands.size(); ++i)
    {
        GUICommand &command = m_Commands[i];
        assert(command.x1 <= command.x2);
        assert(command.y2 <= command.y2);

        SpriteInfo *commandSprite = command.sprite;
        SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

        if (currentSpriteMap != commandSpriteMap || currentVertex  == 5 )
        {
            if (currentVertex > 0)
            {
                //TODO log a warning if we get here by "currentVertex == s_MaxVerticesPerCall"
                //TODO draw all c urrent vertex in the currentBuffer

                m_Materials[currentSpriteMap->MaterialIndex]->Apply();
                m_VertexBuffers[currentSpriteMap->MaterialIndex].UpdateVertex(m_CurrentBufferData, currentVertex);
                m_VertexBuffers[currentSpriteMap->MaterialIndex].Render(RenderManager->GetDeviceContext());
            }
            currentVertex = 0;
            currentSpriteMap = commandSpriteMap;
        }
        int l_Height = RenderManager->GetWindowSize().y;
        int l_Width = RenderManager->GetWindowSize().x;
        float x1 = (command.x1 / (l_Width * 0.5f)) - 1.0f;
        float x2 = (command.x2 / (l_Width * 0.5f)) - 1.0f;
        float y1 = 1.0f - (command.y1 / (l_Height * 0.5f));
        float y2 = 1.0f - (command.y2 / (l_Height * 0.5f));

        float u1 = commandSprite->u1 * (1.0f - command.u1) + commandSprite->u2 * command.u1;
        float u2 = commandSprite->u1 * (1.0f - command.u2) + commandSprite->u2 * command.u2;
        float v1 = commandSprite->v1 * (1.0f - command.v1) + commandSprite->v2 * command.v1;
        float v2 = commandSprite->v1 * (1.0f - command.v2) + commandSprite->v2 * command.v2;


        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y2, 0.f), Vect2f(u1, v2), command.color }; // { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1), command.color };

        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2), command.color };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y1, 0.f), Vect2f(u2, v1), command.color };
    }
    if (currentVertex > 0)
    {

        m_Materials[currentSpriteMap->MaterialIndex]->Apply();
        m_VertexBuffers[currentSpriteMap->MaterialIndex].UpdateVertex(m_CurrentBufferData, currentVertex);
        m_VertexBuffers[currentSpriteMap->MaterialIndex].Render(RenderManager->GetDeviceContext());
    }
    m_Commands.clear();
    m_PanelCommands.clear();
    m_InputUpToDate = false;
}

bool CGUIManager::DoButton(const std::string& guiID, const std::string& buttonID,  CGUIPosition& position)
{
    SpriteInfo* l_sprite;
    CheckInput();
    bool l_result = false;

    if (m_ActiveItem == guiID)
    {
        if (m_MouseWentReleased)
        {
            if (m_HotItem == guiID)
                l_result = true;
            SetNotActive();
        }
    }
    else if (m_HotItem == guiID)
    {
        if (m_MouseWentPressed)
            SetActive(guiID);
    }

    if (IsMouseInside(m_MouseX, m_MouseY, position.Getx(), position.Gety(), position.Getwidth(), position.Getheight()))
        SetHot(guiID);
    else
        SetNotHot(guiID);

    if (m_ActiveItem == guiID && m_HotItem == guiID)
        l_sprite = m_Buttons[buttonID]->GetPressed();
    else
    {
        if (m_HotItem == guiID)
            l_sprite = m_Buttons[buttonID]->GetHighlight();
        else
            l_sprite = m_Buttons[buttonID]->GetNormal();
    }

    GUICommand command =
    {
        l_sprite,
        position.Getx(), position.Gety(), position.Getx() + position.Getwidth(), position.Gety() + position.Getheight(),
        0, 0, 1, 1,
        CColor(1, 1, 1, 1)
    };
    m_Commands.push_back(command);

    return l_result;
}

