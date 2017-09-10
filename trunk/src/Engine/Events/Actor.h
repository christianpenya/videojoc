#pragma once

#ifndef _ENGINE_ACTOR_20170830_H
#define _ENGINE_ACTOR_20170830_H

#include "Utils/Name.h"

class CActor : public CName
{
public:

    CActor() : m_Finished(false) {}
    ~CActor() {}

    virtual void Load(CXMLElement* aElement) = 0;
    virtual void Act() = 0;
    virtual void Update(float elapsedTime) = 0;

    GET_SET_BOOL(Finished);

protected:
    bool m_Finished;
};

#endif //_ENGINE_ACTOR_20170215_H