#include "SceneAnimatedModel.h"

CSceneAnimatedModel::CSceneAnimatedModel(const CXMLElement &TreeNode)
{

}

CSceneAnimatedModel::~CSceneAnimatedModel() {}

bool CSceneAnimatedModel::LoadVertexBuffer()
{
    /**    m_NumVertices = 0;
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
        Vertex::PositionWeightIndicesNormalUV
        *l_Vertexs = (Vertex::PositionWeightIndicesNormalUV
                      *)malloc(m_NumFaces * 3 * sizeof(Vertex::PositionWeightIndicesNormalUV));
        CalIndex *l_Faces = (CalIndex *)malloc(m_NumFaces * 3 * sizeof(CalIndex));
        m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs,
                                            sizeof(Vertex::PositionWeightIndicesNormalUV));
        m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 12,
                                            sizeof(Vertex::PositionWeightIndicesNormalUV));
        m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 28,
                sizeof(Vertex::PositionWeightIndicesNormalUV));
        m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 44,
                                            sizeof(Vertex::PositionWeightIndicesNormalUV));
        m_CalHardwareModel->setTextureCoordNum(1);
        m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 56,
                sizeof(Vertex::PositionWeightIndicesNormalUV));
        m_CalHardwareModel->setIndexBuffer(l_Faces);
        m_CalHardwareModel->load(0, 0, MAXBONES);
        m_NumFaces = m_CalHardwareModel->getTotalFaceCount();
        m_NumVertices = m_CalHardwareModel->getTotalVertexCount();
        CRenderManager &l_RenderManager = CEngine::GetInstance().GetRenderManager();

        m_Geometry = new CTemplatedIndexedGeometry<Vertex::PositionWeightIndicesNormalUV>(new
                CVertexBuffer<Vertex::PositionWeightIndicesNormalUV>(l_RenderManager, l_Vertexs,
                        m_NumVertices), new CIndexBuffer(l_RenderManager, l_Faces, m_NumFaces * 3,
                                sizeof(CalIndex) * 8), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        free(l_Vertexs);
        free(l_Faces);*/
    return true;
}

void CSceneAnimatedModel::LoadMaterials() {}

bool CSceneAnimatedModel::Initialize(CAnimatedCoreModel * AnimatedCoreModel)
{
    //Inicializar los datos necesarios para crear la instacia del Cal3D.

    LoadVertexBuffer();
    LoadMaterials();
}

void CSceneAnimatedModel::Update(float ElapsedTime)
{

}

void Destroy()
{

}

void CSceneAnimatedModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget,
                                        bool AutoLock)
{

}

void CSceneAnimatedModel::BlendCycle(int Id, float Weight, float DelayIn)
{
}

void CSceneAnimatedModel::ClearCycle(int Id, float DelayOut)
{

}

bool CSceneAnimatedModel::IsCycleAnimationActive(int Id) const
{
    return false;
}

bool CSceneAnimatedModel::IsActionAnimationActive(int Id) const
{
    return false;
}