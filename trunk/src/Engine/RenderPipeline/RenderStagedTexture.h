#pragma once
#ifndef _RENDERSTAGEDTEXTURE_H_
#define _RENDERSTAGEDTEXTURE_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"
#include "Utils\TemplatedMapVector.h"
#include "DynamicTexture.h"

class CRenderStagedTexture: public CRenderCmd
{
public:
    CRenderStagedTexture();
    virtual ~CRenderStagedTexture();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM) = 0;
    void CreateRenderTargetViewVector();
    void ActivateTextures ();
private:
    DISALLOW_COPY_AND_ASSIGN(CRenderStagedTexture);
protected:
    class CStagedTexture
    {
    public:
        uint32 m_StageId;
        std::string m_filename;
        CTexture * m_Texture;
        CStagedTexture(uint32 aStageId, CTexture * aTexture);
        void Activate();
    };


    std::vector<CStagedTexture*> m_StagedTextures;
    std::vector<CDynamicTexture *> m_DynamicTextures;
    std::vector<ID3D11RenderTargetView *> m_RenderTargetViews;
    Vect2u m_ViewportPosition;
    Vect2u m_ViewportSize;
};

#endif