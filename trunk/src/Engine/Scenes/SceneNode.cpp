#include "SceneNode.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Render/RenderManager.h"
#include "ConstantBufferManager.h"
#include <d3d11.h>


CSceneNode::CSceneNode() {}

CSceneNode::CSceneNode(const CXMLElement* aElement)
    : CName(aElement->GetAttribute<std::string>("name", ""))
    , CTransform((strcmp(aElement->FirstChildElement()->Name(), "transform") == 0) ? aElement->FirstChildElement() : nullptr)
{
    if (strcmp(aElement->FirstChildElement()->Name(), "transform") == 0)
    {
        tinyxml2::XMLElement const *iTransformNode = aElement->FirstChildElement();
        m_Position = iTransformNode->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f));
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
    Vector4<float> lBSCenter(mBS.GetCenter().x, mBS.GetCenter().y, mBS.GetCenter().z, 0);
    lBSCenter = lRM.GetViewProjectionMatrix() * lBSCenter;
    mBS.SetCenter(Vect3f(lBSCenter.x, lBSCenter.y, lBSCenter.z));

    m_Visible = lRM.m_Frustum->IsVisible(mBS);

    // Todo: Check BS with currentfrustum
    return m_Visible;
}