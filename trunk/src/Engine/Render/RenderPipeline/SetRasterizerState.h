#pragma once
#ifndef _SETRASTERIZERSTATE_H_
#define _SETRASTERIZERSTATE_H_

#include "RenderCmd.h"
#include "Utils\EnumToString.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CSetRasterizerState : public CRenderCmd
{

public:
    CSetRasterizerState();
    virtual ~CSetRasterizerState();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
//    virtual void DrawIMGUI();
private:
    DISALLOW_COPY_AND_ASSIGN(CSetRasterizerState);
    ID3D11RasterizerState* m_RasterizerState;
    int m_CullMode;
    bool m_ClockWise;
    int m_FillMode;
    bool CreateRasterizerState(CRenderManager& lRM);
};


Begin_Enum_String(D3D11_FILL_MODE)
{
    Enum_String_Id(D3D11_FILL_MODE::D3D11_FILL_SOLID, "solid");
    Enum_String_Id(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, "wireframe");
}
End_Enum_String;

Begin_Enum_String(D3D11_CULL_MODE)
{
    Enum_String_Id(D3D11_CULL_MODE::D3D11_CULL_NONE, "none");
    Enum_String_Id(D3D11_CULL_MODE::D3D11_CULL_FRONT, "front");
    Enum_String_Id(D3D11_CULL_MODE::D3D11_CULL_BACK, "back");

}
End_Enum_String;

#endif