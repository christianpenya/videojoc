#pragma once
#ifndef _RENDERPIPELINE_H_
#define _RENDERPIPELINE_H_

#include "Utils\TemplatedMapVector.h"
#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderPipeline : public base::utils::CTemplatedMapVector<CRenderCmd>
{
public:
    CRenderPipeline();
    virtual ~CRenderPipeline();
    bool Load(const std::string& aFilename);
    void Execute();
    void Reload();
    void DrawImgui();
private:
    std::string m_Filename;
};

#endif
