#include "CinematicCameraKey.h"


CCinematicCameraKey::CCinematicCameraKey()
{



}


CCinematicCameraKey::~CCinematicCameraKey()
{


}

bool CCinematicCameraKey::Load(const CXMLElement* aElement)
{
    m_Time = aElement->GetAttribute<float>("time", 0.0f);
    m_Position = aElement->GetAttribute<Vect3f>("pos", Vect3f(0.0f, 0.0f, 0.0f));
    m_LookAt = aElement->GetAttribute<Vect3f>("look_at", Vect3f(0.0f, 0.0f, 0.0f));
    m_Up = aElement->GetAttribute<Vect3f>("up", Vect3f(0.0f, 0.0f, 0.0f));
    m_FOV = aElement->GetAttribute<float>("fov", 0.0f);
    m_NearPlane = aElement->GetAttribute<float>("near_plane", 0.0f);
    m_FarPlane = aElement->GetAttribute<float>("far_plane", 0.0f);
    return true;

}