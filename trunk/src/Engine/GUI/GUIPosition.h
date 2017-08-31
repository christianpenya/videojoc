
#ifndef H_GUI_POSITION_H
#define H_GUI_POSITION_H

#include <Utils/Defines.h>
#include "GUIManager.h"

struct SpriteInfo;
class CGUIPosition
{
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
public:
    CGUIPosition(float _x, float _y, float _w, float _h,
                 CGUIManager::GUIAnchor _anchor = CGUIManager::GUIAnchor::TOP_LEFT,
                 CGUIManager::GUICoordType _anchorCoordsType = CGUIManager::GUICoordType::GUI_ABSOLUTE,
                 CGUIManager::GUICoordType _sizeCorrdsType = CGUIManager::GUICoordType::GUI_ABSOLUTE);
    virtual ~CGUIPosition();

    GET_SET(int, x);
    GET_SET(int, y);
    GET_SET(int, width);
    GET_SET(int, height);
};

#endif //H_GUI_POSITION_H 