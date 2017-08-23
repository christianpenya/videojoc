#include "MathUtils.h"
#include "Transform.h"
#include "XML/XML.h"

CTransform::CTransform(void)
    : m_Position(Vect3f(0, 0, 0))
    , m_PrevPos(Vect3f(0, 0, 0))
    , m_Scale(Vect3f(1.0f, 1.0f, 1.0f))
    , m_Yaw(0.0f)
    , m_Pitch(0.0f)
    , m_Roll(0.0f)
{
}

CTransform::CTransform(const Vect3f &Position)
    : m_Position(Position)
    , m_PrevPos(Position)
    , m_Scale(Vect3f(1.0f, 1.0f, 1.0f))
    , m_Yaw(0.0f)
    , m_Pitch(0.0f)
    , m_Roll(0.0f)
{
}

CTransform::CTransform(const Vect3f &Position, float Yaw, float Pitch, float Roll)
    : m_Position(Position)
    , m_PrevPos(Position)
    , m_Scale(Vect3f(1.0f, 1.0f, 1.0f))
    , m_Yaw(0.0f)
    , m_Pitch(0.0f)
    , m_Roll(0.0f)
{
}

CTransform::CTransform(float Yaw, float Pitch, float Roll)
    : m_Position(Vect3f(0, 0, 0))
    , m_PrevPos(Vect3f(0, 0, 0))
    , m_Scale(Vect3f(1.0f, 1.0f, 1.0f))
    , m_Yaw(0.0f)
    , m_Pitch(0.0f)
    , m_Roll(0.0f)
{
}

CTransform::CTransform(const CXMLElement* aTreeNode)
    : m_Position(aTreeNode->GetAttribute<Vect3f>("position", Vect3f(0.0f, 0.0f, 0.0f)))
    , m_PrevPos(m_Position)
    , m_Scale(aTreeNode->GetAttribute<Vect3f>("scale", Vect3f(1.0f, 1.0f, 1.0f)))
    , m_Yaw(mathUtils::Deg2Rad(aTreeNode->GetAttribute<float>("yaw", 0.0f)))
    , m_Pitch(mathUtils::Deg2Rad(aTreeNode->GetAttribute<float>("pitch", 0.0f)))
    , m_Roll(mathUtils::Deg2Rad(aTreeNode->GetAttribute<float>("roll", 0.0f)))
{
}

CTransform::~CTransform(void)
{
}

const Mat44f & CTransform::GetMatrix()
{
    // Translation
    m_TranslationMatrix.SetIdentity();
    m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

    // Scaling
    m_ScaleMatrix.SetIdentity();
    m_ScaleMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);

    // Rotation
    Mat44f lRotationY;
    Mat44f lRotationX;
    Mat44f lRotationZ;
    lRotationX.SetFromAngleX(-m_Pitch);
    lRotationY.SetFromAngleY(m_Yaw);
    lRotationZ.SetFromAngleZ(m_Roll);

    m_RotationMatrix.SetIdentity();
    m_RotationMatrix = lRotationY * lRotationX * lRotationZ;

    // Acumulate whole transformation in transform matrix
    m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

    return m_TransformMatrix;
}

inline void CTransform::SetPosition(const Vect3f &Position)
{
    m_PrevPos = m_Position;
    m_Position = Position;
}

const Vect3f & CTransform::GetPosition() const
{
    return m_Position;
}

const Vect3f & CTransform::GetPrevPosition() const
{
    return m_PrevPos;
}

Vect3f CTransform::GetForward() const
{
    return  Vect3f( mathUtils::Cos(m_Yaw) * mathUtils::Cos(m_Pitch),
                    mathUtils::Sin(m_Pitch),
                    mathUtils::Sin(m_Yaw) * mathUtils::Cos(m_Pitch));
}

Vect3f CTransform::GetUp() const
{
    return  Vect3f(-mathUtils::Cos(m_Yaw) * mathUtils::Sin(m_Pitch),
                   mathUtils::Cos(m_Pitch),
                   -mathUtils::Sin(m_Yaw) * mathUtils::Sin(m_Pitch));
}

void CTransform::SetForward(const Vect3f& aForward)
{
    SetYaw(mathUtils::ATan2(aForward.x, aForward.z));
    SetPitch(mathUtils::ATan2(aForward.y, mathUtils::Sqrt(aForward.z * aForward.z + aForward.x * aForward.x)));
    SetPitch(0.0f);
    SetRoll(0.0f);
}