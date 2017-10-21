#pragma once
#ifndef H_GUISPRITE_H
#define H_GUISPRITE_H

#include <Utils\Defines.h>

struct SpriteInfo;

class CGUISPrite
{
private:
    SpriteInfo* m_Normal;

public:
    CGUISPrite();
    CGUISPrite(SpriteInfo* _normal);
    virtual ~CGUISPrite();
    GET_SET(SpriteInfo*, Normal);
};

#endif //H_BUTON_H