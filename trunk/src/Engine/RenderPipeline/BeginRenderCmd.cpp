#pragma once
#include "BeginRenderCmd.h"
#include "XML\XML.h"

CBeginRenderCmd::CBeginRenderCmd()
{

}

CBeginRenderCmd::~CBeginRenderCmd()
{

}

//Leerá el nodo:
//<begin_render name="begin_render_main_loop"/>

bool CBeginRenderCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}

void CBeginRenderCmd::Execute(CRenderManager& lRM)
{
    lRM.BeginRender();
}