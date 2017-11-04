#pragma once

#ifndef _ENGINE_LOADSCENEREACTOR_20171020_H
#define _ENGINE_LOADSCENEREACTOR_20171020_H

#include "Events/Reactor.h"

class CXMLElement;

class CLoadSceneReactor : public CReactor
{

public:
    CLoadSceneReactor();
    ~CLoadSceneReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);

private:
    std::string m_Scene;

};

#endif //_ENGINE_LOADSCENEREACTOR_20171020_H
