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

    void DrawImgui();

private:

    std::string mFilename;
    bool mEnabled;

};

#endif //_ENGINE_EVENTMANAGER_20170830_H