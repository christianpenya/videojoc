#pragma once

#ifndef _ENGINE_EVENT_20170830_H
#define _ENGINE_EVENT_20170830_H

#include "Utils/Name.h"
#include "Utils/Active.h"

class CXMLElement;
class CActor;
class CReactor;

class CEvent : public CName, public CActive
{
public:
    CEvent();
    CEvent(CXMLElement* aElement);
    ~CEvent();

    void Start();
    void Update();

    bool mIsEnCours;

private:
    CActor* mActor;
    CReactor* mReactor;

    bool mAlreadyReacting;
};

#endif //_ENGINE_EVENT_20170830_H