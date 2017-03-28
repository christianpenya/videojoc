#pragma once
#ifndef _RENDERSTAGEDTEXTURE_H_
#define _RENDERSTAGEDTEXTURE_H_

#include "RenderCmd.h"
#include <vector>
#include "DynamicTexture.h"
#include "Graphics/Materials/MaterialManager.h"
#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderStagedTexture: public CRenderCmd
{
public:
    CRenderStagedTexture();
    virtual ~CRenderStagedTexture();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM) = 0;
    void CreateRenderTargetViewVector();
    void ActivateTextures();
    void AddDynamicTextureMaterial(CDynamicTexture* DynamicTexture, CMaterial* Material);
private:
    DISALLOW_COPY_AND_ASSIGN(CRenderStagedTexture);
protected:
    class CStagedTexture
    {
    public:
        uint32 m_StageId;
        std::string m_filename;
        CTexture* m_Texture;
        CStagedTexture(uint32 aStageId, CTexture* aTexture);
        void Activate();
    };

    class CDynamicTextureMaterial
    {
    public:
        CDynamicTexture* m_DynamicTexture;
        CMaterial* m_Material;

        CDynamicTextureMaterial(CDynamicTexture* DynamicTexture, CMaterial* Material):
            m_DynamicTexture(DynamicTexture),
            m_Material (Material)
        {}

        void Activate(int StageId);
    };

    std::vector<CStagedTexture*> m_StagedTextures;
    std::vector<CDynamicTextureMaterial *> m_DynamicTexturesMaterials;
    std::vector<ID3D11RenderTargetView *> m_RenderTargetViews;
    Vect2u m_ViewportPosition;
    Vect2u m_ViewportSize;
};

#endif
