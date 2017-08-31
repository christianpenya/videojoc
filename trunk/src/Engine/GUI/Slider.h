#ifndef H_SLIDER_H
#define H_SLIDER_H

#include <Utils/Defines.h>
#include "GUIManager.h"

class CSlider
{
private:
    SpriteInfo* m_Base;
    SpriteInfo* m_Top;
    SpriteInfo* m_Handle;
    SpriteInfo* m_PressedHandle;
public:
    CSlider(SpriteInfo* _Base, SpriteInfo* _Top, SpriteInfo* _Handle, SpriteInfo* PressedHandle);
    ~CSlider();
    GET_SET(SpriteInfo*, Base)
    GET_SET(SpriteInfo*, Top)
    GET_SET(SpriteInfo*, Handle)
    GET_SET(SpriteInfo*, PressedHandle)
    float handleRelativeWidth;
    float handleRelativeHeight;
};

#endif //H_SLIDER_H