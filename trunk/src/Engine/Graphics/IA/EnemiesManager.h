#pragma once
#ifndef _ENEMIESMANAGER_H_
#define _ENEMIESMANAGER_H_

#include "Utils\TemplatedMapVector.h"
#include "enemy.h"
#include "Math\Vector4.h"


class CEnemiesManager : public base::utils::CTemplatedMapVector<CEnemy>
{
public:
    CEnemiesManager();
    ~CEnemiesManager();
    bool Load(const std::string& aFileName);
    bool ReLoad();
    void DrawImgui();
private:
    std::string m_LevelEnemiesFilename;
    bool Load();
};

#endif