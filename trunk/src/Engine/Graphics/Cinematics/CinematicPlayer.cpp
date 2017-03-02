#include "CinematicPlayer.h"

CCinematicPlayer::CCinematicPlayer() :
    m_CurrentKey{0},
    m_NextKey(0)
{}

CCinematicPlayer::~CCinematicPlayer()
{
    CTemplatedMapVector<CCinematicKey>::Destroy();
}

void CCinematicPlayer::PlayFoward(float currentTime)
{
    GetCurrentKeyForward(currentTime);
    float l_CurrentP = (currentTime - m_ResourcesVector[m_CurrentKey]->GetTime()) / (m_ResourcesVector[m_NextKey]->GetTime() - m_ResourcesVector[m_CurrentKey]->GetTime());
    Apply(l_CurrentP / 100, m_ResourcesVector[m_CurrentKey], m_ResourcesVector[m_NextKey]);
    return;
}

void CCinematicPlayer::PlayBackward(float currentTime)
{
    GetCurrentKeyBackward(currentTime);
    float l_CurrentP = (currentTime - m_ResourcesVector[m_CurrentKey]->GetTime()) / (m_ResourcesVector[m_NextKey]->GetTime() - m_ResourcesVector[m_CurrentKey]->GetTime());
    Apply(l_CurrentP / 100, m_ResourcesVector[m_CurrentKey], m_ResourcesVector[m_NextKey]);
    return;
}


void CCinematicPlayer::GetCurrentKeyForward(float currentTime)
{
    m_NextKey = m_CurrentKey + 1;
    if (m_ResourcesVector[m_CurrentKey]->GetTime() <= currentTime)
    {
        if (m_NextKey < m_ResourcesVector.size())
        {
            if (m_ResourcesVector[m_NextKey]->GetTime() > currentTime)
            {
                return;
            }
            else
            {
                m_CurrentKey += 1;
                GetCurrentKeyForward(currentTime);
                return;
            }
        }
        else
        {
            m_NextKey -= 1;
            m_CurrentKey -= 1;
            return;
        }
    }
    else
    {
        m_CurrentKey = 0;
        GetCurrentKeyForward(currentTime);
        return;
    }
}


void CCinematicPlayer::GetCurrentKeyBackward(float currentTime)
{
    m_NextKey = m_CurrentKey - 1;
    if (m_ResourcesVector[m_CurrentKey]->GetTime() >= currentTime)
    {
        if (m_NextKey > 0)
        {
            if (m_ResourcesVector[m_NextKey]->GetTime() < currentTime)
            {
                return;
            }
            else
            {
                m_CurrentKey -= 1;
                GetCurrentKeyBackward(currentTime);
                return;
            }
        }
        else
        {
            m_NextKey += 1;
            m_CurrentKey += 1;
            return;
        }
    }
    else
    {
        m_CurrentKey = m_ResourcesVector.size() - 1;
        GetCurrentKeyBackward(currentTime);
        return;
    }
}

/*
// Calculate the % of the animation, in order to interpolate key by key
float l_CurrentP = (currentTime - mKeys[m_CurrentKey]->GetTime()) /
(mKeys[m_NextKey]->GetTime() - mKeys[m_CurrentKey]->GetTime());
Apply(l_CurrentP, mKeys[m_CurrentKey], mKeys[m_NextKey]);
MVUAB –Importación y Exportación– Cinemáticas 6
// Especialización por reprocutor
CAnimator<Vect3f> lPositionAnimator;
lPositionAnimator.Init(A->GetPosition(), B->GetPosition(), 1.0f, FUNC_LINEAR);
Vect3f lCurrentPosition;
lPositionAnimator.Update(aPercentage, lCurrentPosition);
*/
