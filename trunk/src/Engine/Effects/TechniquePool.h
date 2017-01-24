#pragma once
#ifndef _ENGINE_TECHNIQUEPOOL_20170107
#define _ENGINE_TECHNIQUEPOOL_20170107

#include "Effect.h"
#include "Utils\TemplatedMapVector.h"

class CTechniquePool : public base::utils::CTemplatedMapVector<CEffect>, public CName
{
public:
    CTechniquePool(CXMLElement* aElement);
    virtual ~CTechniquePool();

private:
    DISALLOW_COPY_AND_ASSIGN(CTechniquePool);
};

#endif //_ENGINE_TECHNIQUEPOOL_20170107
