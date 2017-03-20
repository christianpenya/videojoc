#pragma once
#ifndef __CQUAD_HH__
#define __CQUAD_HH__

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CGeometry;
class CEffect;

class CQuad
{
public:
    CQuad();
    virtual ~CQuad();

    bool Init();
    bool Render();
    bool Render(CEffect *Effect);

private:
    CGeometry* mGeometry;
};

#endif
