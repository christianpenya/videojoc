#include "CinematicPlayer.h"

CCinematicPlayer::CCinematicPlayer(): m_CurrentKey(nullptr), m_NextKey(nullptr)
{
}

CCinematicPlayer::~CCinematicPlayer()
{


}


void CCinematicPlayer::PlayFoward(float currentTime)
{
    if (m_NextKey->GetTime() >= currentTime)
    {
        m_CurrentKey = m_ResourcesMap.begin()->second.m_Value;
    }
    if (m_NextKey->GetTime() <= currentTime)
    {
        m_CurrentKey = m_NextKey;
    }
    for (TMapResources::iterator iKeyMapEntry = m_ResourcesMap.begin(); iKeyMapEntry != m_ResourcesMap.end(); ++iKeyMapEntry)
    {
        if ((*iKeyMapEntry).second.m_Value == m_CurrentKey)
        {
            ++iKeyMapEntry;
            if (iKeyMapEntry != m_ResourcesMap.end())
            {
                m_NextKey = (*iKeyMapEntry).second.m_Value;
            }
            else
            {
                m_NextKey = (*m_ResourcesMap.begin()).second.m_Value;
                return;
            }
            float l_CurrentP = (currentTime - m_CurrentKey->GetTime()) /(m_NextKey->GetTime() - m_CurrentKey->GetTime());
            Apply(l_CurrentP/100, m_CurrentKey, m_NextKey);

            return;
        }
    }
}

void CCinematicPlayer::PlayBackward(float currentTime)
{
    if (m_NextKey->GetTime() <= currentTime)
    {
        m_CurrentKey = m_ResourcesMap.begin()->second.m_Value;
    }
    if (m_NextKey->GetTime() >= currentTime)
    {
        m_CurrentKey = m_NextKey;
    }
    for (TMapResources::iterator iKeyMapEntry = m_ResourcesMap.end(); iKeyMapEntry != m_ResourcesMap.begin(); --iKeyMapEntry)
    {
        if ((*iKeyMapEntry).second.m_Value == m_CurrentKey)
        {
            --iKeyMapEntry;
            if (iKeyMapEntry != m_ResourcesMap.begin())
                m_NextKey = (*iKeyMapEntry).second.m_Value;
            else
                m_NextKey = (*m_ResourcesMap.end()).second.m_Value;
            float l_CurrentP = (currentTime - m_CurrentKey->GetTime())/(m_NextKey->GetTime() - m_CurrentKey->GetTime());
            Apply(l_CurrentP/100, m_CurrentKey, m_NextKey);
            return;
        }
    }

}


void CCinematicPlayer::GetCurrentKeyForward(float currentTime)
{



}


void CCinematicPlayer::GetCurrentKeyBackward(float currentTime)
{

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
