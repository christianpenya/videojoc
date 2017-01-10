#pragma once
#ifndef _ENGINE_AXISALIGNEDBOUNDINGBOX_20161227
#define _ENGINE_AXISALIGNEDBOUNDINGBOX_20161227

#include "Base\Math\Vector3.h"
#include "Base\Utils\Defines.h"

class CAxisAlignedBoundingBox 
{
public:   
    CAxisAlignedBoundingBox(){}
    virtual ~CAxisAlignedBoundingBox(){}
    GET_SET_REF(Vect3f, Min);   GET_SET_REF(Vect3f, Max);

protected:   
    Vect3f m_Min;   
    Vect3f m_Max;
};

#endif //_ENGINE_AXISALIGNEDBOUNDINGBOX_20161227