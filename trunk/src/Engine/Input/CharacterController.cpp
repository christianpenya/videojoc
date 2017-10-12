#include "CharacterController.h"
#include "Engine/Engine.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Camera/CameraManager.h"

#include "Physx/PhysxManager.h"
#include "Utils/CheckedRelease.h"

CCharacterController::~CCharacterController()
{
    __H_CHECKED_RELEASE__(sceneManager);
    __H_CHECKED_RELEASE__(actionManager);
    __H_CHECKED_RELEASE__(physXManager);
}

void CCharacterController::Update(float ElapsedTime)
{
    CCameraController* cam = &CEngine::GetInstance().GetCameraManager().GetCurrentCamera();
    Vect3f l_Front = cam->GetFront();
    Vect3f l_Up = cam->GetUp();
    Vect3f l_Right = l_Front ^ l_Up;

    l_Front.y = 0;
    l_Right.y = 0;
    float x = (*actionManager)("x_move")->value;
    float z = (*actionManager)("z_move")->value;
    float run = (*actionManager)("run")->value;
    float crouch = (*actionManager)("crouch")->value;
    float l_Speed = m_Speed;
    if (run>0.f && z>0.1f)
    {
        x /= 2;
        l_Speed *= 1.4;
    }
    if (crouch>0.1f)
        l_Speed *= .6f;



    Vect3f forwardMove = z * l_Front;
    Vect3f horizontalMove = x * l_Right;
    // m_Movement = { x, 0.0f, z };
    m_Movement = forwardMove + horizontalMove;
    if (m_Movement != (0, 0, 0))
    {

        player->BlendCycle(2, 1, 0);

        m_Movement = m_Movement.GetNormalized();
        Vect3f l_Dir = m_Movement;

        m_Movement *= l_Speed * ElapsedTime;
        //m_Position = m_Position + m_Movement;
        physXManager->MoveCharacterController("player", m_Movement, PHYSX_UPDATE_STEP);
        if (player != nullptr)
        {
            //player->SetPosition(m_Position);
            m_Position = physXManager->GetActorPosition("player");
            player->SetPosition(m_Position);
            player->SetForward(l_Dir);
        }


    }
    else
    {
        if (crouch > 0.1f)
        {
            if (!player->IsCycleAnimationActive(1))
            {
                player->ClearCycle(0, 0.5);
                player->BlendCycle(1, 0.5, 0);
            }

        }
        else
        {
            if (!player->IsCycleAnimationActive(0))
            {
                player->ClearCycle(1, 0.5);
                player->BlendCycle(0, 0.5, 0);

            }
        }


        physXManager->MoveCharacterController("player", m_Movement, PHYSX_UPDATE_STEP);
        if (player != nullptr)
        {
            //player->SetPosition(m_Position);
            m_Position = physXManager->GetActorPosition("player");
            player->SetPosition(m_Position);

        }
    }

}

void CCharacterController::Init(CSceneManager* sceneManager)
{
    actionManager = &CEngine::GetInstance().GetActionManager();
    physXManager = &CEngine::GetInstance().GetPhysXManager();
    sceneManager = &CEngine::GetInstance().GetSceneManager();

    if (sceneManager->GetCurrentScene()->Exist("LPlayer"))
    {
        CLayer* layer = sceneManager->GetCurrentScene()->GetLayer("LPlayer");
        if (layer->Exist("player"))
        {
            CSceneAnimatedModel* anim = (CSceneAnimatedModel*)(*layer)("player");
            player = anim;
        }
    }

    if (player != nullptr)
        //    player->SetPosition(m_Position);
        m_Position = player->GetPosition();


}

