#pragma once

#ifndef _ENGINE_REACTOR_20170830_H
#define _ENGINE_REACTOR_20170830_H

#include "Utils/Name.h"
#include "Utils\EnumToString.h"

class CReactor : public CName
{
public:

    enum EReactorType
    {
        eDumb,
        eOpenDoor,
        eReloadScene,
        eLoadScene,
        eReactorTypeCount,
        eTextGUIR,
        eTextIcon,
        eAudioTrigger,
        eDeactivateObj
    };

    CReactor() : m_Finished(false) {}
    ~CReactor() {}

    virtual void Load(CXMLElement* aElement) = 0;
    virtual void React() = 0;
    virtual void Update(float elapsedTime) = 0;

    GET_SET_BOOL(Finished);
    EReactorType mReactorType;

protected:
    bool m_Finished;
};

Begin_Enum_String(CReactor::EReactorType)
{
    Enum_String_Id(CReactor::eDumb, "dumb");
    Enum_String_Id(CReactor::eOpenDoor, "open_door");
    Enum_String_Id(CReactor::eReloadScene, "reload_scene");
    Enum_String_Id(CReactor::eLoadScene, "load_scene");
    Enum_String_Id(CReactor::eTextGUIR, "textGUIR");
    Enum_String_Id(CReactor::eTextIcon, "iconText");
    Enum_String_Id(CReactor::eAudioTrigger, "audio_triggerR");
    Enum_String_Id(CReactor::eDeactivateObj, "DeactivateObj")
}
End_Enum_String;

#endif //_ENGINE_REACTOR_20170215_H