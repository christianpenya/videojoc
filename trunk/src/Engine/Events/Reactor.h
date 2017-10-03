#pragma once

#ifndef _ENGINE_REACTOR_20170830_H
#define _ENGINE_REACTOR_20170830_H

#include "Utils/Name.h"

class CReactor : public CName
{
public:
    CReactor() : m_Finished(false) {}
    ~CReactor() {}

    virtual void Load(CXMLElement* aElement) = 0;
    virtual void React() = 0;
    virtual void Update(float elapsedTime) = 0;

    GET_SET_BOOL(Finished);

protected:
    bool m_Finished;
};

#endif //_ENGINE_REACTOR_20170215_H