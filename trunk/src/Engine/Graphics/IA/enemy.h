#pragma once
#ifndef _ENEMY_20170820_H
#define _ENEMY_20170820_H

#include <memory>
#include "Utils\TemplatedMap.h"
#include "Graphics/Mesh/Mesh.h"
#include <iostream>

#include "Utils/TemplatedMapVector.h"
#include "Math/Color.h"
#include "Utils/StringUtils.h"
//#include "Graphics/Scenes/SceneNode.h"
#include "Graphics/Scenes/SceneMesh.h"
#include "Utils/EnumToString.h"
#include "Render/RenderPipeline/DynamicTexture.h"
#include "Graphics/Scenes/Layer.h"


using namespace std;
enum Input { PATROL, CHASE };

class CEnemy;
class CPatrolingState;
class CChasingState;
class CEnemyState
{
public:
    static std::shared_ptr<CPatrolingState> patroling;
    static std::shared_ptr<CChasingState> chasing;
    virtual ~CEnemyState() = default;
    virtual void handleInput(CEnemy&, Input) = 0;
    virtual void update(CEnemy&) = 0;
};

class CPatrolingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemy&, Input) override;
    virtual void update(CEnemy&) override;
};

class CChasingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemy&, Input) override;
    virtual void update(CEnemy&) override;
};


class CEnemy : public CSceneMesh
{

private:
    //std::string name;
    std::shared_ptr<CEnemyState> state;

    virtual void updateWithNewState()
    {
        state->update(*this);
    }

public:
    enum EEnemyType
    {
        eDron = 0,
        eGuard = 1
    };

    CEnemy(CXMLElement* aElement, EEnemyType aEnemyType);
    virtual ~CEnemy();


    GET_SET(EEnemyType, EnemyType);
    //CMesh* mMesh;
    std::string getName()
    {
        return m_Name;
    }

    EEnemyType m_EnemyType;
    void DrawImgui();
    bool Update(float ElapsedTime);

    virtual void handleInput(Input input)
    {
        state->handleInput(*this, input);
    }
    void changeState(std::shared_ptr<CEnemyState> newState)
    {
        state = newState;
        updateWithNewState();
    }
    virtual void patrol()
    {
        std::cout << GetName() << "patrol" << std::endl;
    }
    void chase()
    {
        std::cout << GetName() << "chase" << std::endl;
    }

};

Begin_Enum_String(CEnemy::EEnemyType)
{
    Enum_String_Id(CEnemy::eDron, "dron");
    Enum_String_Id(CEnemy::eGuard, "guard");
}
End_Enum_String;

#endif

/*int main()
{
    Enemy dron("Dron");
    dron.patrol();

    int choice=2;

    auto chooseAction = [&choice](Enemy& enemy)
    {
        const Input input1 = static_cast<Input>(choice - 1);
        enemy.handleInput(input1);
    };
    while (true)
    {
        chooseAction(dron);
    }

}


*/