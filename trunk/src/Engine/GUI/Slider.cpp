#include "Slider.h"

#include "GUIManager.h"
CSlider::CSlider(SpriteInfo* _Base, SpriteInfo* _Top, SpriteInfo* _Handle, SpriteInfo* PressedHandle)
{
    m_Base = _Base;
    m_Top = _Top;
    m_Handle = _Handle;
    m_PressedHandle = PressedHandle;
    float handle_x1 = m_Handle->u1 * m_Handle->SpriteMap->w;
    float handle_x2 = m_Handle->u2 * m_Handle->SpriteMap->w;
    float handle_y1 = m_Handle->v1 * m_Handle->SpriteMap->h;
    float handle_y2 = m_Handle->v2 * m_Handle->SpriteMap->h;
    float base_x1 = m_Base->u1 * m_Base->SpriteMap->w;
    float base_x2 = m_Base->u2 * m_Base->SpriteMap->w;
    float base_y1 = m_Base->v1 * m_Base->SpriteMap->h;
    float base_y2 = m_Base->v2 * m_Base->SpriteMap->h;

    handleRelativeWidth = (handle_x2 - handle_x1) / (base_x2 - base_x1);
    handleRelativeHeight = (handle_y2 - handle_y1) / (base_y2 - base_y1);
}

CSlider::~CSlider()
{
}