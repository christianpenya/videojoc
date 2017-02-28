#pragma once
#ifndef _ENGINE_FRUSTUM_20170118_H
#define _ENGINE_FRUSTUM_20170118_H

#include "Math/Matrix44.h"

class CBoundingSphere;
class CAxisAlignedBoundingBox;

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CFrustum
{
public:
    CFrustum() {}
    virtual ~CFrustum() {}
    void Update(const Mat44f &ViewProj);
    bool IsVisible(const CBoundingSphere &aBoundingSphere) const;
    bool IsVisible(const CAxisAlignedBoundingBox &aAABB) const;

private:
    float m_Proj[16];
    float m_Modl[16];
    float m_Clip[16];
    float m_Frustum[6][4];
};


#endif //_ENGINE_FRUSTUM_20170118_H