#pragma once
#ifndef _ENGINE_CINEMATIC_OBJECT_PLAYER_20170213
#define _ENGINE_CINEMATIC_OBJECT_PLAYER_20170213

#include "CinematicPlayer.h"
#include "Scenes/SceneNode.h"

class CCinematicObjectPlayer : public CCinematicPlayer
{
public:
    CCinematicObjectPlayer();
    virtual ~CCinematicObjectPlayer();
    virtual bool Load(const CXMLElement* aElement);
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicObjectPlayer);
    CSceneNode* m_SceneNode;
protected:
    virtual void Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B);
};

#endif