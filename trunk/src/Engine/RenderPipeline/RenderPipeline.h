#pragma once
#ifndef _RENDERPIPELINE_H_
#define _RENDERPIPELINE_H_

#include "Utils\TemplatedMapVector.h"
#include "RenderCmd.h"

class CRenderPipeline : public base::utils::CTemplatedMapVector<CRenderCmd>
{
public:
    CRenderPipeline();
    virtual ~CRenderPipeline();
    bool Load(const std::string& aFilename);
    void Execute();
    void Reload();
private:
    std::string m_Filename;
};

#endif
