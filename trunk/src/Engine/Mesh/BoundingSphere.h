#pragma once
#ifndef _ENGINE_BOUNDINGSPHERE_20161222
#define _ENGINE_BOUNDINGSPHERE_20161222

#include "Math\Vector3.h"
#include "Utils\Defines.h"

class CBoundingSphere
{
public:
    CBoundingSphere() : m_Radius(0.0) {   }
    virtual ~CBoundingSphere()   {   }

    GET_SET_REF(Vect3f, Center);
    GET_SET_REF(float, Radius);

protected:
    float m_Radius;
    Vect3f m_Center;
};

#endif //_ENGINE_BOUNDINGSPHERE_20161222