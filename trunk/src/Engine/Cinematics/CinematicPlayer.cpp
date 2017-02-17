#include "CinematicPlayer.h"

CCinematicPlayer::CCinematicPlayer()
{


}

CCinematicPlayer::~CCinematicPlayer()
{


}


void CCinematicPlayer::PlayFoward(float currentTime)
{


}

void CCinematicPlayer::PlayBackward(float currentTime)
{


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
