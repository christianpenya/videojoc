#include "CinematicObjectKey.h"


CCinematicObjectKey::CCinematicObjectKey()
{



}


CCinematicObjectKey::~CCinematicObjectKey()
{


}

bool CCinematicObjectKey::Load(const CXMLElement* aElement)
{
    m_Time = aElement->GetAttribute<float>("time", 0.0f);
    m_Position = aElement->GetAttribute<Vect3f>("pos", Vect3f(0.0f, 0.0f, 0.0f));
    float yaw = aElement->GetAttribute<float>("yaw",0.0f);
    float pitch = aElement->GetAttribute<float>("pitch", 0.0f);
    float roll = aElement->GetAttribute<float>("roll", 0.0f);
    m_Rotation = Vect3f(yaw, pitch, roll);
    m_Scale = aElement->GetAttribute<Vect3f>("scale", Vect3f(0.0f, 0.0f, 0.0f));

    return true;

}