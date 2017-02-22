#pragma once
#ifndef _ENGINE_SCENEANIMATEDMODEL_20170117
#define _ENGINE_SCENEANIMATEDMODEL_20170117

#include "Scenes\SceneNode.h"
#include <vector>

class CalModel;
class CAnimatedCoreModel;
class CalHardwareModel;
class CMaterial;
class CGeometry;
class CRenderManager;

class CSceneAnimatedModel : public CSceneNode
{
private:
    CalModel *m_CalModel;
    CAnimatedCoreModel *m_AnimatedCoreModel;
    CalHardwareModel *m_CalHardwareModel;
    std::vector<CMaterial *> m_Materials;
    CGeometry *m_Geometry;
    int m_NumVertices;
    int m_NumFaces;
    bool LoadVertexBuffer();
    void LoadMaterials();
public:
    CSceneAnimatedModel(const CXMLElement &TreeNode);
    virtual ~CSceneAnimatedModel();
    bool Initialize(CAnimatedCoreModel *AnimatedCoreModel);
    bool Render(CRenderManager &RenderManager);
    bool Update(float ElapsedTime);
    void Destroy();
    void ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget = 1.0f,
                       bool AutoLock = true);
    void BlendCycle(int Id, float Weight, float DelayIn);
    void ClearCycle(int Id, float DelayOut);
    bool IsCycleAnimationActive(int Id) const;
    bool IsActionAnimationActive(int Id) const;
};


#endif //_ENGINE_SCENEANIMATEDMODEL_20170117
