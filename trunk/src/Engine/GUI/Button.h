#ifndef H_BUTON_H
#define H_BUTON_H

#include "Utils\Defines.h"

class SpriteInfo;

class CButon
{
private:
    SpriteInfo* m_Normal;
    SpriteInfo* m_Highlight;
    SpriteInfo* m_Pressed;
public:
    CButon();
    CButon(SpriteInfo* _normal, SpriteInfo* _highlight, SpriteInfo* _pressed);
    virtual ~CButon();
    GET_SET(SpriteInfo*, Normal);
    GET_SET(SpriteInfo*, Highlight);
    GET_SET(SpriteInfo*, Pressed);
};

#endif //H_BUTON_H