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

    void React();
    void Update();
};

#endif //_ENGINE_DUMBREACTOR_20170831_H
