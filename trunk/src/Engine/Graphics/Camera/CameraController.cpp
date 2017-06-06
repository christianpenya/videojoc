#include "CameraController.h"
#include "Engine\Engine.h"

void CCameraController::Update(float ElapsedTime)
{
    //.CEngine.GetInstance().GetSceneManager()

}

Vect4f CCameraController::GetPositionInScreenCoordinates(const Vect3f &Position) const
{
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    Mat44f l_ViewProj = lRM.GetViewProjectionMatrix();
    Vect4f l_Pos4f(Position.x, Position.y, Position.z, 1.0);
    float x = l_Pos4f.x*l_ViewProj.m00 + l_Pos4f.y*l_ViewProj.m10 + l_Pos4f.z*l_ViewProj.m20 + l_ViewProj.m30;
    float y = l_Pos4f.x*l_ViewProj.m01 + l_Pos4f.y*l_ViewProj.m11 + l_Pos4f.z*l_ViewProj.m21 + l_ViewProj.m31;
    float z = l_Pos4f.x*l_ViewProj.m02 + l_Pos4f.y*l_ViewProj.m12 + l_Pos4f.z*l_ViewProj.m22 + l_ViewProj.m32;
    float w = l_Pos4f.x*l_ViewProj.m03 + l_Pos4f.y*l_ViewProj.m13 + l_Pos4f.z*l_ViewProj.m23 + l_ViewProj.m33;
    if (w == 0)
        w = 1;
    return Vect4f(x / w, y / w, 0, 0);
}