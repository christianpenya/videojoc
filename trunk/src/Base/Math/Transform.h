#pragma once
#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include "Utils/Defines.h"

#include "Math/Vector3.h"
#include "Math/Matrix44.h"

XML_FORWARD_DECLARATION

class CTransform
{
public:
    CTransform();
    CTransform(const Vect3f &Position);
    CTransform(const Vect3f &Position, float Yaw, float Pitch, float Roll);
    CTransform(float Yaw, float Pitch, float Roll);
    CTransform(const CXMLElement* XMLTreeNode);
    virtual ~CTransform();
    virtual void SetPosition(const Vect3f &Position);
    const Vect3f & GetPosition() const;
    const Vect3f & GetPrevPosition() const;
    GET_SET_REF(Vect3f, Scale)
    const Mat44f & GetMatrix();

    GET_SET(float,Yaw)
    GET_SET(float,Pitch)
    GET_SET(float,Roll)

    Vect3f GetForward() const;
    Vect3f GetUp() const;
    
    void SetForward(const Vect3f& aForward );

protected:
    Vect3f   m_Position, m_PrevPos, m_Scale;
    float    m_Yaw, m_Pitch, m_Roll;
    Mat44f   m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix, m_ScaleMatrix;
};

#endif //__H_TRANSFORM__