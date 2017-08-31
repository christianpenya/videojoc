#pragma once
#ifndef _ENGINE_CINEMATIC_PLAYER_20170213
#define _ENGINE_CINEMATIC_PLAYER_20170213

#include "CinematicKey.h"
#include "Utils\TemplatedMapVector.h"
#include <sstream>

XML_FORWARD_DECLARATION

class CCinematicPlayer: public base::utils::CTemplatedMapVector< CCinematicKey >
{
public:
    CCinematicPlayer();
    virtual ~CCinematicPlayer() ;
    virtual bool Load(const CXMLElement* aElement) = 0;
    void PlayFoward(float currentTime); //TODO
    void PlayBackward(float currentTime);//TODO
    virtual void Start() = 0;
    virtual void Finish() = 0;
    std::string mName;
protected:

    size_t m_CurrentKey, m_NextKey ;
    //CCinematicKey *m_CurrentKey, *m_NextKey;
    void GetCurrentKeyForward(float currentTime);
    void GetCurrentKeyBackward(float currentTime);
    virtual void Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B) = 0;
    std::string Convert(float number)
    {
        std::ostringstream buff;
        buff << number;
        return buff.str();
    }
private:
    DISALLOW_COPY_AND_ASSIGN(CCinematicPlayer);
};

#endif