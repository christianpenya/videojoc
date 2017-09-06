#pragma once

#ifndef _ENGINE_OPENDOORREACTOR_20170831_H
#define _ENGINE_OPENDOORREACTOR_20170831_H

#include "Events/Reactor.h"

class CXMLElement;

class COpenDoorReactor : public CReactor
{
private:

public:
    COpenDoorReactor();
    ~COpenDoorReactor();

    void React();
    void Update();
};

#endif //_ENGINE_OPENDOORREACTOR_20170831_H
