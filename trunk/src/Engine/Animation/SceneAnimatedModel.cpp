#include "SceneAnimatedModel.h"
#include "AnimatedCoreModel.h"
#include "cal3d.h"
#include "Mesh/VertexTypes.h"
#include "Engine/Engine.h"
#include "Mesh/TemplatedIndexedGeometry.h"
#include "Scenes/ConstantBufferManager.h"
#include "Utils/CheckedDelete.h"
#include "Materials/Material.h"
#include "Math/MathTypes.h"
#include "Math/Quaternion.h"
#include "Effects/Effect.h"
#include "Effects/Technique.h"

CSceneAnimatedModel::CSceneAnimatedModel(const CXMLElement &TreeNode)
    : CSceneNode(&TreeNode)
{

}

CSceneAnimatedModel::~CSceneAnimatedModel() {}

bool CSceneAnimatedModel::LoadVertexBuffer()
{
    m_NumVertices = 0;
    m_NumFaces = 0;

    CalCoreModel *l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
    for (int i = 0; i<l_CalCoreModel->getCoreMeshCount(); ++i)
    {
        CalCoreMesh *l_CalCoreMesh = l_CalCoreModel->getCoreMesh(i);
        for (int j = 0; j<l_CalCoreMesh->getCoreSubmeshCount(); ++j)
        {
            CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
            m_NumVertices += l_CalCoreSubmesh->getVertexCount();
            m_NumFaces += l_CalCoreSubmesh->getFaceCount();
        }
    }
    VertexTypes::PositionWeightIndicesNormalUV *l_Vertexs = (VertexTypes::PositionWeightIndicesNormalUV *)malloc(m_NumFaces * 3 * sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    CalIndex *l_Faces = (CalIndex *)malloc(m_NumFaces * 3 * sizeof(CalIndex));
    m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs, sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 12, sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 28, sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 44, sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    m_CalHardwareModel->setTextureCoordNum(1);
    m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 56, sizeof(VertexTypes::PositionWeightIndicesNormalUV));
    m_CalHardwareModel->setIndexBuffer(l_Faces);
    m_CalHardwareModel->load(0, 0, MAXBONES);
    m_NumFaces = m_CalHardwareModel->getTotalFaceCount();
    m_NumVertices = m_CalHardwareModel->getTotalVertexCount();
    CRenderManager &l_RenderManager = CEngine::GetInstance().GetRenderManager();

    m_Geometry = new CTemplatedIndexedGeometry<VertexTypes::PositionWeightIndicesNormalUV>(new
            CVertexBuffer<VertexTypes::PositionWeightIndicesNormalUV>(l_RenderManager, l_Vertexs,
                    m_NumVertices), new CIndexBuffer(l_RenderManager, l_Faces, m_NumFaces * 3,
                            sizeof(CalIndex) * 8), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    free(l_Vertexs);
    free(l_Faces);
    return true;
}

void CSceneAnimatedModel::LoadMaterials() {}

bool CSceneAnimatedModel::Initialize(CAnimatedCoreModel * AnimatedCoreModel)
{
    //Inicializar los datos necesarios para crear la instacia del Cal3D.
    m_AnimatedCoreModel = AnimatedCoreModel;
    CalCoreModel *l_CalCoreModel = AnimatedCoreModel->GetCoreModel();
    m_CalModel = new CalModel(l_CalCoreModel);

    for (int i = 0; i < l_CalCoreModel->getCoreMeshCount(); ++i)
        m_CalModel->attachMesh(i);

    m_CalHardwareModel = new CalHardwareModel(m_CalModel->getCoreModel());//Aqui o mes amun?

    LoadVertexBuffer();
    LoadMaterials();

    BlendCycle(0, 1.0f, 0.0f);
    m_CalModel->update(0.0f);

    const std::vector<CMaterial *> &l_Materials = m_AnimatedCoreModel->GetMaterials();

    for (size_t i = 0; i < l_Materials.size(); ++i)
        m_Materials.push_back(l_Materials[i]);

    return false;
}

bool CSceneAnimatedModel::Update(float ElapsedTime)
{
    m_CalModel->update(ElapsedTime);
    return false;
}

void CSceneAnimatedModel::Destroy()
{
    base::utils::CheckedDelete(m_CalModel);

    base::utils::CheckedDelete(m_AnimatedCoreModel);
    for (size_t	i = 0; i < m_Materials.size(); ++i)
        base::utils::CheckedDelete(m_Materials[i]);
    m_Materials.clear();
    base::utils::CheckedDelete(m_Geometry);
}

void CSceneAnimatedModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{
    m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut, WeightTarget, AutoLock);
}

void CSceneAnimatedModel::BlendCycle(int Id, float Weight, float DelayIn)
{
    m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CSceneAnimatedModel::ClearCycle(int Id, float DelayOut)
{
    m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}

bool CSceneAnimatedModel::IsCycleAnimationActive(int Id) const
{

    return false;
}

bool CSceneAnimatedModel::IsActionAnimationActive(int Id) const
{
    return false;
}

bool CSceneAnimatedModel::Render(CRenderManager &RenderManager)
{
    if (m_CalHardwareModel == nullptr)
        return false;
    CConstantBufferManager &l_CB = CEngine::GetInstance().GetConstantBufferManager();
    l_CB.mObjDesc.m_World = GetMatrix();
    l_CB.BindBuffer(RenderManager.GetDeviceContext(),
                    CConstantBufferManager::CB_Object);
    ID3D11DeviceContext* l_DeviceContext = RenderManager.GetDeviceContext();
    for (int l_HardwareMeshId = 0; l_HardwareMeshId<m_CalHardwareModel ->getHardwareMeshCount(); ++l_HardwareMeshId)
    {
        CMaterial *l_Material = m_Materials[l_HardwareMeshId];
        if (l_Material != nullptr && l_Material->GetTechnique() != nullptr && l_Material->GetTechnique()->GetEffect() != nullptr)
        {
            l_Material->Apply();
            m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
            Mat44f l_Transformations[MAXBONES];
            for (int l_BoneId = 0; l_BoneId<m_CalHardwareModel->getBoneCount();
                    ++l_BoneId)
            {
                Quatf l_Quaternion = (const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
                l_Transformations[l_BoneId].SetIdentity();
                l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
                CalVector translationBoneSpace = m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
                l_Transformations[l_BoneId].SetPos(Vect3f(translationBoneSpace.x,
                                                   translationBoneSpace.y, translationBoneSpace.z));
            }
            memcpy(&l_CB.mAnimatedModelDesc, l_Transformations,
                   MAXBONES*sizeof(float) * 4 * 4);
            l_CB.BindBuffer(RenderManager.GetDeviceContext(),
                            CConstantBufferManager::CB_AnimatedModel);
            m_Geometry->RenderIndexed(l_DeviceContext, m_CalHardwareModel->getFaceCount() * 3, m_CalHardwareModel->getStartIndex(), m_CalHardwareModel->getBaseVertexIndex());
        }
    }
    return true;
}


