#pragma once

#ifndef _ENGINE_ACTOR_20170830_H
#define _ENGINE_ACTOR_20170830_H

#include "Utils/Name.h"

class CActor : public CName
{
public:

    CActor() : mIsFinished(false) {}
    ~CActor() {}

    virtual void Act() = 0;
    virtual void Update(float elapsedTime) = 0;

    bool IsFinished()
    {
        return mIsFinished;
    }

protected:
    bool mIsFinished;
};

#endif //_ENGINE_ACTOR_20170215_H