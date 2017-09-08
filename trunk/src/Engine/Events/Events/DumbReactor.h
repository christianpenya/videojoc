#pragma once

#ifndef _ENGINE_DUMBREACTOR_20170831_H
#define _ENGINE_DUMBREACTOR_20170831_H

#include "Events/Reactor.h"

class CXMLElement;

class CDumbReactor : public CReactor
{

private:

public:
    CDumbReactor();
    ~CDumbReactor();

    void Load(CXMLElement* aElement);
    void React();
    void Update(float elapsedTime);
};

#endif //_ENGINE_DUMBREACTOR_20170831_H
