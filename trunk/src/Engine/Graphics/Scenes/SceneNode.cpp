#include "SceneNode.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Render/RenderManager.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "ImGUI/imgui.h"

CSceneNode::CSceneNode():
    m_Visible(false),
    m_ignoreFrustum(false),
    m_NodeType(ESceneNodeType::eSceneNodeCount)
{}

CSceneNode::CSceneNode(const CXMLElement* aElement)
    : CTransform((strcmp(aElement->FirstChildElement()->Name(), "transform") == 0) ? aElement->FirstChildElement() : nullptr),
      CName(aElement->GetAttribute<std::string>("name", "")),
      m_ignoreFrustum(false),
      CActive(aElement->GetAttribute<bool>("active", false)),
      m_Visible(false),
      m_NodeType(ESceneNodeType::eSceneNodeCount)
{
    if (strcmp(aElement->FirstChildElement()->Name(), "transform") == 0)
    {
        tinyxml2::XMLElement const* iTransformNode = aElement->FirstChildElement();
        m_PrevPos = iTransformNode->GetAttribute<Vect3f>("forward", Vect3f(0.0f, 0.0f, 1.0f));
    }
}

CSceneNode::~CSceneNode() {}

bool CSceneNode::Update(float aDeltaTime)
{
    return true;
}

bool CSceneNode::Render(CRenderManager& lRM)
{
    if (m_ignoreFrustum)
    {
        return true;
    }
    else
    {
        Vector4<float> lBSCenter(mBS.GetCenter().x, mBS.GetCenter().y, mBS.GetCenter().z, 0);
        lBSCenter = lRM.GetViewProjectionMatrix() * lBSCenter;
        mBS.SetCenter(Vect3f(lBSCenter.x, lBSCenter.y, lBSCenter.z));
        m_Visible = lRM.m_Frustum->IsVisible(mBS);

        return m_Visible;
    }
}

void CSceneNode::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat3("Forward", (float*)&m_PrevPos, -100.0f, 100.0f);
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, -100.0f, 100.0f);
    }
}
