#ifndef __CPlane_HH__
#define __CPlane_HH__

#pragma once

#include "Mesh.h"

class CGeometry;
class CMaterial;

class CPlane : public CMesh
{
public:
  CPlane();
  virtual ~CPlane();
};

#endif
