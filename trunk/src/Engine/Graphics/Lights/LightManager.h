#pragma once
#ifndef _LIGHTMANAGER_H_
#define _LIGHTMANAGER_H_

#include "Utils\TemplatedMapVector.h"
#include "Light.h"
#include "Math\Vector4.h"

class CLightManager : public base::utils::CTemplatedMapVector<CLight>
{
public:
    CLightManager();
    ~CLightManager();
    bool Load(const std::string& aFileName);
    void SetLightConstants(size_t idLight, CLight* alight);
    void SetLightsConstants();
    bool ReLoad();
    void DrawImgui();
    GET_SET(Vect4f, AmbientLightColor)
private:
    Vect4f m_AmbientLightColor;
    std::string m_LevelLightsFilename;
    bool Load();
};

#endif