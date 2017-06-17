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
#include "GUIPosition.h"


CGUIManager::CGUIManager(): m_MouseX(0), m_MouseY(0)
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

    for (size_t i = 0; i < m_VertexBuffers.size(); i++)
        base::utils::CheckedDelete(m_VertexBuffers[i]);
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
                        if (strcmp(iSubElement->Name(), "material")==0)
                        {
                            m_Materials.push_back(new CMaterial(iSubElement));
                            m_VertexBuffers.push_back( new CGeometryTriangleList<VertexTypes::SpriteVertex>(
                                                           new CVertexBuffer<VertexTypes::SpriteVertex>(aRM,nullptr, MAX_VERTICES_PER_CALL, true)
                                                       )
                                                     );
                        }
                        else if (strcmp(iSubElement->Name(), "sprite") == 0)
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
                else if (iElement->Name() == std::string("font"))
                {
                    std::string l_FontName = iElement->GetAttribute<std::string>("name","");
                    std::string l_FileName = iElement->GetAttribute<std::string>("path","");

                    /* CXMLDocument document2;
                     EXMLParseError error2 = document2.LoadFile(m_FileName.c_str());
                     if (base::xml::SucceedLoad(error2))
                     {

                         CXMLElement* iElement2 = document2.FirstChildElement("font");
                         CXMLElement* iElement2Aux;
                         CXMLElement* iElement2Aux2;

                         if (iElement2!=NULL)
                         {
                             iElement2Aux = iElement2->FirstChildElement();
                             while (iElement2Aux != NULL)
                             {
                                 if (iElement2Aux->Name() == std::string("common"))
                                 {
                                     m_LineHeightPerFont[l_FontName] = iElement2Aux->GetAttribute("lineHeight",0);
                                     m_BasePerFont[l_FontName] = iElement2Aux->GetAttribute("base",0);
                                 }
                                 else if (iElement2Aux->Name() == std::string("pages"))
                                 {
                                     iElement2Aux2 = iElement2Aux->FirstChildElement();
                                     while (iElement2Aux2!=NULL)
                                     {
                                         if (iElement2Aux2->Name() == std::string("page"))
                                             m_TexturePerFont[l_FontName].push_back(&m_Sprites[iElement2Aux2->GetAttribute<std::string>("file","")]);
                                         iElement2Aux2 = iElement2Aux2->NextSiblingElement();
                                     }
                                 }
                                 else if (iElement2Aux->Name() == std::string("chars"))
                                 {
                                     iElement2Aux2 = iElement2Aux->FirstChildElement();
                                     while (iElement2Aux2 != NULL)
                                     {
                                         if (iElement2Aux2->Name() == std::string("char"))
                                         {
                                             FontChar l_FontChar = { iElement2Aux2->GetAttribute("x", 0), iElement2Aux2->GetAttribute("y", 0), iElement2Aux2->GetAttribute("width", 0), iElement2Aux2->GetAttribute("height",0),
                                                                     iElement2Aux2->GetAttribute("xoffset",0), iElement2Aux2->GetAttribute("yoffset",0), iElement2Aux2->GetAttribute("xadvance",0), iElement2Aux2->GetAttribute("page",0), iElement2Aux2->GetAttribute("chnl",0)
                                                                   };
                                             m_CharactersPerFont[l_FontName][iElement2Aux2->GetAttribute("id",0)] = l_FontChar;
                                         }
                                         iElement2Aux2 = iElement2Aux2->NextSiblingElement();
                                     }
                                 }
                                 else if (iElement2Aux->Name() == std::string("kernings"))
                                 {
                                     iElement2Aux2 = iElement2Aux->FirstChildElement();
                                     while (iElement2Aux2 != NULL)
                                     {
                                         if (iElement2Aux2->Name() == std::string("kerning"))
                                             m_KerningsPerFont[l_FontName][iElement2Aux2->GetAttribute("first",0)][iElement2Aux2->GetAttribute("second",0)] = iElement2Aux2->GetAttribute("second",0);
                                         iElement2Aux2 = iElement2Aux2->NextSiblingElement();
                                     }
                                 }
                                 iElement2Aux = iElement2Aux->NextSiblingElement();
                             }

                         }
                     }*/
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

    for (size_t i = 0; i < m_Commands.size(); ++i)
    {
        GUICommand &command = m_Commands[i];
        assert(command.x1 <= command.x2);
        assert(command.y2 <= command.y2);

        SpriteInfo *commandSprite = command.sprite;
        SpriteMapInfo *commandSpriteMap = commandSprite->SpriteMap;

        if (currentSpriteMap != commandSpriteMap || currentVertex == MAX_VERTICES_PER_CALL)
        {
            if (currentVertex > 0)
            {
                //TODO log a warning if we get here by "currentVertex == s_MaxVerticesPerCall"
                //TODO draw all c urrent vertex in the currentBuffer

                m_Materials[currentSpriteMap->MaterialIndex]->Apply();
                m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertex(m_CurrentBufferData, currentVertex);
                m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(RenderManager->GetDeviceContext(), currentVertex);
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


        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y2, 0.f), Vect2f(u1, v2)}; // { Vect4f(x1, y2, 0.f, 1.f), command.color, Vect2f(u2, v2) };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1) };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2)};


        m_CurrentBufferData[currentVertex++] = { Vect3f(x1, y1, 0.f), Vect2f(u1, v1)};
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y1, 0.f), Vect2f(u2, v1) };
        m_CurrentBufferData[currentVertex++] = { Vect3f(x2, y2, 0.f), Vect2f(u2, v2)};

    }
    if (currentVertex > 0)
    {

        m_Materials[currentSpriteMap->MaterialIndex]->Apply();
        m_VertexBuffers[currentSpriteMap->MaterialIndex]->UpdateVertex(m_CurrentBufferData, currentVertex);
        m_VertexBuffers[currentSpriteMap->MaterialIndex]->Render(RenderManager->GetDeviceContext(), currentVertex);
    }
    m_Commands.clear();
    m_InputUpToDate = false;
}

int CGUIManager::FillCommandQueueWithTextAux(const std::string& _font, const std::string& _text, const CColor& _color, Vect4f *textBox_)
{
    Vect4f dummy;
    if (textBox_ == nullptr) textBox_ = &dummy;

    *textBox_ = Vect4f(0, 0, 0, 0);


    assert(m_LineHeightPerFont.find(_font) != m_LineHeightPerFont.end());
    assert(m_BasePerFont.find(_font) != m_BasePerFont.end());
    assert(m_CharactersPerFont.find(_font) != m_CharactersPerFont.end());
    assert(m_KerningsPerFont.find(_font) != m_KerningsPerFont.end());
    assert(m_TexturePerFont.find(_font) != m_TexturePerFont.end());


    int lineHeight = m_LineHeightPerFont[_font];
    int base = m_BasePerFont[_font];
    const std::unordered_map< wchar_t, FontChar > &l_CharacterMap = m_CharactersPerFont[_font];
    const std::unordered_map< wchar_t, std::unordered_map< wchar_t, int>> &l_Kernings = m_KerningsPerFont[_font];
    const std::vector<SpriteInfo*> &l_TextureArray = m_TexturePerFont[_font];

    wchar_t last = 0;

    int cursorX = 0, cursorY = 0;

    float spritewidth = (float)l_TextureArray[0]->SpriteMap->w;
    float spriteHeight = (float)l_TextureArray[0]->SpriteMap->h;

    int addedCommands = 0;

    for (char c : _text)
    {
        if (c == '\n')
        {
            cursorY += lineHeight;
            cursorX = 0;
            last = 0;
        }
        else
        {
            auto it = l_CharacterMap.find((wchar_t)c);
            if (it != l_CharacterMap.end())
            {
                const FontChar &fontChar = it->second;
                auto it1 = l_Kernings.find(last);
                if (it1 != l_Kernings.end())
                {
                    auto it2 = it1->second.find(c);
                    if (it2 != it1->second.end())
                    {
                        int kerning = it2->second;
                        cursorX += kerning;
                    }
                }
                GUICommand command = {};
                command.sprite = l_TextureArray[fontChar.page];
                command.x1 = cursorX + fontChar.xoffset;
                command.x2 = command.x1 + fontChar.width;
                command.y1 = cursorY - base + fontChar.yoffset;
                command.y2 = command.y1 + fontChar.height;

                command.u1 = (float)fontChar.x / spritewidth;
                command.u2 = (float)(fontChar.x + fontChar.width) / spritewidth;
                command.v1 = (float)fontChar.y / spriteHeight;
                command.v2 = (float)(fontChar.y + fontChar.height) / spriteHeight;

                command.color = _color;

                m_Commands.push_back(command);
                ++addedCommands;

                last = c;
                cursorX += fontChar.xadvance;

                if (command.x1 < textBox_->x) textBox_->x = (float)command.x1;
                if (command.y1 < textBox_->y) textBox_->y = (float)command.y1;
                if (command.x2 > textBox_->z) textBox_->z = (float)command.x2;
                if (command.y2 > textBox_->w) textBox_->w = (float)command.y2;
            }
        }
    }

    return addedCommands;
}

void CGUIManager::FillCommandQueueWithText(const std::string& _font, const std::string& _text,
        Vect2f _coord, GUIAnchor _anchor, const CColor& _color)
{
    Vect4f textSizes;

    int numCommands = FillCommandQueueWithTextAux(_font, _text, _color, &textSizes);
    Vect2f adjustment = _coord;

    if ((int)_anchor & (int)GUIAnchor::TOP)
        adjustment.y -= textSizes.y;
    else if ((int)_anchor & (int)GUIAnchor::MID)
        adjustment.y -= (textSizes.y + textSizes.w) * 0.5f;
    else if ((int)_anchor & (int)GUIAnchor::BOTTOM)
        adjustment.y -= textSizes.w;
    else
        assert(false);

    if ((int)_anchor & (int)GUIAnchor::LEFT)
        adjustment.x -= textSizes.x;
    else if ((int)_anchor & (int)GUIAnchor::CENTER)
        adjustment.x -= (textSizes.x + textSizes.z) * 0.5f;
    else if ((int)_anchor & (int)GUIAnchor::RIGHT)
        adjustment.x -= textSizes.z;
    else
        assert(false);

    for (size_t i = m_Commands.size() - numCommands; i < m_Commands.size(); ++i)
    {
        m_Commands[i].x1 += (int)adjustment.x;
        m_Commands[i].x2 += (int)adjustment.x;
        m_Commands[i].y1 += (int)adjustment.y;
        m_Commands[i].y2 += (int)adjustment.y;
    }
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

std::string CGUIManager::DoTextBox(const std::string& guiID, const std::string& _font, const std::string& currentText, CGUIPosition position)
{
    CheckInput();

    if (IsMouseInside(m_MouseX, m_MouseY, position.Getx(), position.Gety(), position.Getwidth(), position.Getheight()))
        SetHot(guiID);
    else
        SetNotHot(guiID);

    if (m_ActiveItem == guiID)
    {
        if (m_MouseWentReleased)
        {
            if (m_HotItem == guiID)
            {
                SetSelected(guiID);
            }
            SetNotActive();
        }
    }
    else if (m_HotItem == guiID)
    {
        if (m_MouseWentPressed)
            SetActive(guiID);
    }

    std::string displayText;
    std::string activeText = currentText;

    if (guiID == m_SelectedItem)
    {
        //CKeyboardInput* l_KeyBoard = CEngine::GetInstance()->GetInputManager()->GetKeyBoard();
        /*
         *  if (l_KeyBoard->isConsumible())
         {
         wchar_t lastChar = l_KeyBoard->ConsumeLastChar();
         if (lastChar >= 0x20 && lastChar < 255)
         {
         activeText += (char)lastChar;
         }
         else if (lastChar == '\r')
         {
         activeText += '\n';
         }
         else if (lastChar == '\b')
         {
         if (activeText.length() > 2)
         activeText = activeText.substr(0, activeText.length() - 1);
         }
         }
         }

         FillCommandQueueWithText(_font, activeText, Vect2f(position.Getx() + position.Getwidth() * 0.05f,
         position.Gety() + position.Getheight() * 0.75f));*/

    }
    return activeText;
}
