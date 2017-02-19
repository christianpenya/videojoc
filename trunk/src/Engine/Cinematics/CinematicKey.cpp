#include "CinematicKey.h"


CCinematicKey::CCinematicKey()
{



}


CCinematicKey::~CCinematicKey()
{


}

bool CCinematicKey::Load(const CXMLElement* aElement)
{
    m_Time = aElement->GetAttribute<float>("time", 0.0f);
    m_Position = aElement->GetAttribute<Vect3f>("pos", Vect3f(0.0f, 0.0f, 0.0f));


    return true;

}