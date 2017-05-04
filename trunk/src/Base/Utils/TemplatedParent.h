#pragma once
#ifndef __H_TEMPLATED_PARENT__20170423
#define __H_TEMPLATED_PARENT__20170423

#include "Defines.h"
#include "CheckedDelete.h"

XML_FORWARD_DECLARATION

template<class T>
class CTemplatedParent
{
public:
    CTemplatedParent() {}
    CTemplatedParent(T& aTemplatedParent)
    {
        m_Parent = aTemplatedParent;
    }

    virtual ~CTemplatedParent() {}

    GET_SET(T*, Parent);

protected:
    T* m_Parent;
};

#endif // __H_TEMPLATED_PARENT__20170423
