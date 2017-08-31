#pragma once
#ifndef __H_ACTIVE__
#define __H_ACTIVE__

#include "Defines.h"

class CActive
{
public:
    CActive();
    CActive(bool aEnable);
    virtual ~CActive();

    GET_SET_REF(bool, Active);
    bool m_Active;
protected:

};

#endif // __H_ACTIVE__