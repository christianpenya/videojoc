#pragma once

#ifndef _ENGINE_EVENTMANAGER_20170830_H
#define _ENGINE_EVENTMANAGER_20170830_H

#include "Utils\TemplatedMapVector.h"
#include "Event.h"

class CEventManager : public base::utils::CTemplatedMapVector<CEvent>
{
public:
    CEventManager();
    virtual ~CEventManager();

    bool Load(std::string aFilename);
    bool Load();
    void Update(float elapsedTime);

    CEvent* GetEvent(std::string aEvent);
    CActor* GetActor(std::string aActor);
    CReactor* GetReactor(std::string aReactor);

private:
    std::string mFilename;

    base::utils::CTemplatedMapVector<CActor> mActors;
    base::utils::CTemplatedMapVector<CReactor> mReactors;
};

#endif //_ENGINE_EVENTMANAGER_20170830_H
