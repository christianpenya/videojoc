#pragma once
#ifndef _SETALPHABLENDSTATE_H_
#define _SETALPHABLENDSTATE_H_

#include "RenderCmd.h"
#include "Utils\EnumToString.h"

class CSetAlphaBlendState : public CRenderCmd
{
public:
    CSetAlphaBlendState();
    virtual ~CSetAlphaBlendState();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CSetAlphaBlendState);
    ID3D11BlendState *m_AlphaBlendState;
    bool m_enabled;
    D3D11_BLEND m_SrcBlend;
    D3D11_BLEND m_DestBlend;
    D3D11_BLEND_OP m_BlendOp;
    D3D11_BLEND m_SrcBlendAlpha;
    D3D11_BLEND m_DestBlendAlpha;
    D3D11_BLEND_OP m_BlendOpAlpha;
    D3D11_COLOR_WRITE_ENABLE m_render_target_write_mask;
    bool CreateAlphaBlendState(CRenderManager& lRM);
};

Begin_Enum_String(D3D11_BLEND)
{
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR, "BlendFactor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_DEST_ALPHA, "DestAlpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_DEST_COLOR, "DestColor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR, "InvBlendFactor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA, "InvDestAlpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR, "InvDestColor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_SRC1_ALPHA, "InvSrc1Alpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_SRC1_COLOR, "InvSrc1Color");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA, "InvSrcAlpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR, "InvSrcColor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_SRC1_ALPHA, "Src1Alpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_SRC1_COLOR, "Src1Color");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_SRC_ALPHA, "SrcAlpha");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_SRC_ALPHA_SAT, "SrcAlphaSat");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_SRC_COLOR, "SrcColor");
    Enum_String_Id(D3D11_BLEND::D3D11_BLEND_ZERO, "Zero");
}
End_Enum_String;

Begin_Enum_String(D3D11_BLEND_OP)
{
    Enum_String_Id(D3D11_BLEND_OP::D3D11_BLEND_OP_ADD, "add");
    Enum_String_Id(D3D11_BLEND_OP::D3D11_BLEND_OP_MAX, "max");
    Enum_String_Id(D3D11_BLEND_OP::D3D11_BLEND_OP_MIN, "min");
    Enum_String_Id(D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT, "revSubtract");
    Enum_String_Id(D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT, "subtract");
}
End_Enum_String;

Begin_Enum_String(D3D11_COLOR_WRITE_ENABLE)
{
    Enum_String_Id(D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL, "all");
    Enum_String_Id(D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALPHA, "alpha");
    Enum_String_Id(D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_BLUE, "blue");
    Enum_String_Id(D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_GREEN, "green");
    Enum_String_Id(D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_RED, "red");

}
End_Enum_String;

#endif