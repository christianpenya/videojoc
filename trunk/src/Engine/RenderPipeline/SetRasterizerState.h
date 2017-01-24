#pragma once
#ifndef _SETRASTERIZERSTATE_H_
#define _SETRASTERIZERSTATE_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"
#include "Utils\EnumToString.h"


class CSetRasterizerState : public CRenderCmd
{
public:
    enum EFillMode
    {
        eSolid=0,
        eWireFrame=1
    };
public:
    CSetRasterizerState();
    virtual ~CSetRasterizerState();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawIMGUI();
private:
    DISALLOW_COPY_AND_ASSIGN(CSetRasterizerState);
    ID3D11RasterizerState* m_RasterizerState;
    int m_CullMode;
    bool m_ClockWise;
    int m_FillMode;
    bool CreateRasterizerState();
};


Begin_Enum_String(D3D11_FILL_MODE)
{
    Enum_String_Id(D3D11_FILL_MODE::D3D11_FILL_SOLID, "solid");
    Enum_String_Id(D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, "wireframe");
}
End_Enum_String;


#endif