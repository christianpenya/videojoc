#pragma once

#ifndef _ENGINE_REACTOR_20170830_H
#define _ENGINE_REACTOR_20170830_H

#include "Utils/Name.h"

class CReactor : public CName
{
public:
    CReactor() : mIsFinished(false) {}
    ~CReactor() {}

    virtual void Load(CXMLElement* aElement) = 0;
    virtual void React() = 0;
    virtual void Update(float elapsedTime) = 0;

    bool IsFinished()
    {
        return mIsFinished;
    }

protected:
    bool mIsFinished;
};

#endif //_ENGINE_REACTOR_20170215_H