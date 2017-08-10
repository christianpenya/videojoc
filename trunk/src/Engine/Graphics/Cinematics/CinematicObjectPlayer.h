#pragma once
#ifndef _ENGINE_CINEMATIC_OBJECT_PLAYER_20170213
#define _ENGINE_CINEMATIC_OBJECT_PLAYER_20170213

#include "CinematicPlayer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "CinematicObjectKey.h"

class CCinematicObjectPlayer : public CCinematicPlayer
{
public:
    CCinematicObjectPlayer();
    virtual ~CCinematicObjectPlayer();
    virtual bool Load(const CXMLElement* aElement);
    virtual void Start() {}
    virtual void Finish() {}
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicObjectPlayer);
    CSceneNode* m_SceneNode;
    CSceneNode* GetSceneNode(std::string sceneName, std::string layerName, std::string nodeName);
protected:
    virtual void Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B);
};

#endif