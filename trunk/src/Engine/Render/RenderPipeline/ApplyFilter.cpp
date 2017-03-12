#include "ApplyFilter.h"

CApplyFilter::CApplyFilter()
{

}

CApplyFilter::~CApplyFilter()
{

}

bool CApplyFilter::Load(const CXMLElement* aElement)
{
    bool lOk = CDrawQuad::Load(aElement);
    return lOk;
}


void CApplyFilter::Execute(CRenderManager &lRM)
{

}