#include "ParticleSystemInstance.h"
#include "Graphics/Effects/Technique.h"
#include "Graphics/Effects/EffectManager.h"
#include "ParticleManager.h"
#include "Engine/engine.h"
#include "XML\XML.h"
#include "Graphics\Mesh\TemplatedGeometry.h"
#include "Graphics\Mesh\VertexBuffer.h"
#include "Graphics\Buffers\ConstantBufferManager.h"

CParticleSystemInstance::CParticleSystemInstance()
{

}

CParticleSystemInstance::~CParticleSystemInstance()
{

}

float CParticleSystemInstance::GetRandomValue(float min, float max)
{

    float a = m_UnitDistribution(m_RandomEngine);
    float value = mathUtils::Lerp(min, max, a);
    return value;
}


Vect3f CParticleSystemInstance::GetRandomValue(Vect3f min, Vect3f max)
{
    float a1 = m_UnitDistribution(m_RandomEngine);
    float a2 = m_UnitDistribution(m_RandomEngine);
    float a3 = m_UnitDistribution(m_RandomEngine);
    Vect3f value;
    value.x = mathUtils::Lerp(min.x, max.x, a1);
    value.y = mathUtils::Lerp(min.y, max.x, a2);
    value.z = mathUtils::Lerp(min.z, max.x, a3);
    return value;
}


Vect2f CParticleSystemInstance::GetRandomValue(Vect2f min, Vect2f max)
{
    float a1 = m_UnitDistribution(m_RandomEngine);
    float a2 = m_UnitDistribution(m_RandomEngine);
    Vect2f value;
    value.x = mathUtils::Lerp(min.x, max.x, a1);
    value.y = mathUtils::Lerp(min.y, max.x, a2);
    return value;
}


CColor CParticleSystemInstance::GetRandomValue(CColor min, CColor max)
{
    float a = m_UnitDistribution(m_RandomEngine);
    CColor value = min.Lerp(max, a);
    return value;
}

float CParticleSystemInstance::GetRandomValue(Vect2f value)
{
    return GetRandomValue(value.x, value.y);
}


CParticleSystemInstance::CParticleSystemInstance(const CXMLElement* aElement)
    : CSceneNode(aElement)
    , m_RandomEngine(rnd())
    , m_UnitDistribution(0.0f, 1.0f)
    , m_NextParticleEmission(aElement->GetAttribute<float>("nextParticleEmission", 0.2f))
    , m_Awake (aElement->GetAttribute<bool>("awake", true))
    , m_AwakeTimer(aElement->GetAttribute<float>("awakeTimer", 0.1f))
    , m_ActiveParticles(aElement->GetAttribute<int>("activeParticles", 200))
    , m_EmissionBoxHalfSize(aElement->GetAttribute<Vect3f>("emission_box_size", Vect3f(1.0f,1.0f,1.0f))*0.5f)
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

    CParticleManager& lParticleManager = CEngine::GetInstance().GetParticleManager();
    m_Type = lParticleManager(aElement->GetAttribute<std::string>("name", ""));
    m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
    m_EmissionScaler = m_Type->m_EmitAbsolute ? 1 : 1.0f / m_EmissionVolume;
    CVertexBuffer<VertexTypes::ParticleVertex> *l_buffer = new CVertexBuffer < VertexTypes::ParticleVertex>(lRenderManager, m_ParticleRenderableData, s_MaxParticlesPerInstance);
    m_Vertices = new CGeometryPointList<VertexTypes::ParticleVertex>(l_buffer);
}


float CParticleSystemInstance::ComputeTimeToNextParticle()
{
    float particlesPerSecPerM3 = GetRandomValue(m_Type->m_EmitRate);
    return m_EmissionScaler / particlesPerSecPerM3;
}

void CParticleSystemInstance::UpdateState(float ElapsedTime)
{
    m_AwakeTimer -= ElapsedTime;
    while (m_AwakeTimer < 0)
    {
        m_Awake = !m_Awake;
        m_AwakeTimer += GetRandomValue(m_Awake ? m_Type->m_AwakeTime : m_Type->m_SleepTime);
    }
}

bool CParticleSystemInstance::Update(float ElapsedTime)
{
    if (m_Awake)
    {
        m_NextParticleEmission -= ElapsedTime;
        while (m_NextParticleEmission < 0)
        {
            if (m_ActiveParticles < s_MaxParticlesPerInstance)
            {
                ParticleData particle = {};
                particle.Position = GetRandomValue(-m_EmissionBoxHalfSize, m_EmissionBoxHalfSize);
                particle.DistanceToCamera = GetDistanceToCamera(particle.Position);
                particle.Velocity = GetRandomValue(m_Type->m_StartingSpeed1, m_Type->m_StartingSpeed2);
                particle.Acceleration = GetRandomValue(m_Type->m_StartingAcceleration1, m_Type->m_StartingAcceleration2);

                particle.SizeControlPoint = 0;
                particle.LastSizeControlTime = 0;
                particle.LastSize = GetRandomValue(m_Type->m_ControlPointSizes[0].m_Size);
                particle.NextSizeControlTime = m_Type->m_ControlPointSizes.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Time);
                particle.NextSize = m_Type->m_ControlPointSizes.size() < 2 ? particle.LastSize : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Size);
                particle.Angle = GetRandomValue(m_Type->GetStartingAngle());
                particle.AngularAcceleration = GetRandomValue(m_Type->m_StartingAccelerationAngle);
                particle.AngularSpeed = GetRandomValue(m_Type->m_StartingAngularSpeed);


                particle.ColorControlPoint = 0;
                particle.LastColorControlTime = 0;
                particle.LastColor = GetRandomValue(m_Type->m_ControlPointColors[0].m_Color1, m_Type->m_ControlPointColors[0].m_Color2);
                particle.NextColorControlTime = m_Type->m_ControlPointColors.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointColors[1].m_Time);
                particle.NextColor = m_Type->m_ControlPointColors.size() < 2 ? particle.LastColor : GetRandomValue(m_Type->m_ControlPointColors[1].m_Color1, m_Type->m_ControlPointColors[1].m_Color2);

                particle.CurrentFrame = 0;
                particle.TimeToNextFrame = m_Type->m_TimePerFrame;
                particle.Lifetime = 0;
                particle.TotalLife = GetRandomValue(m_Type->m_Life);

                m_ParticleData[m_ActiveParticles] = particle;
                ++m_ActiveParticles;
            }
            m_NextParticleEmission += ComputeTimeToNextParticle();
        }
    }


    for (int i = 0; i<m_ActiveParticles; ++i)
    {
        ParticleData *particle = &m_ParticleData[i];
        particle->Position += particle->Velocity * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->Acceleration;
        particle->DistanceToCamera = GetDistanceToCamera(particle->Position);
        particle->Velocity += particle->Acceleration * ElapsedTime;
        particle->TimeToNextFrame -= ElapsedTime;
        particle->Lifetime += ElapsedTime;

        while (particle->TimeToNextFrame < 0 && (m_Type->m_LoopFrames || particle->CurrentFrame < m_Type->m_NumFrames - 1))
        {
            particle->CurrentFrame = (particle->CurrentFrame + 1) % m_Type->m_NumFrames;
            particle->TimeToNextFrame += m_Type->m_TimePerFrame;
        }

        while (particle->Lifetime > particle->NextSizeControlTime && particle->Lifetime < particle->TotalLife)
        {
            ++particle->SizeControlPoint;
            particle->LastSize = particle->NextSize;
            particle->LastSizeControlTime = particle->NextSizeControlTime;

            if (particle->SizeControlPoint + 1 < m_Type->m_ControlPointSizes.size())
            {
                particle->NextSize = GetRandomValue(m_Type->m_ControlPointSizes[particle->SizeControlPoint + 1].m_Size);
                particle->NextSizeControlTime = GetRandomValue(m_Type->m_ControlPointSizes[particle->SizeControlPoint + 1].m_Time);
            }
            else
            {
                particle->NextSizeControlTime = particle->TotalLife;
            }
        }

        while (particle->Lifetime > particle->NextColorControlTime && particle->Lifetime < particle->TotalLife)
        {
            ++particle->ColorControlPoint;
            particle->LastColor = particle->NextColor;
            particle->LastColorControlTime = particle->NextColorControlTime;

            if (particle->ColorControlPoint + 1 < m_Type->m_ControlPointColors.size())
            {
                particle->NextColor = GetRandomValue(m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Color1, m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Color2);
                particle->NextColorControlTime = GetRandomValue(m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Time);
            }
            else
            {
                particle->NextColorControlTime = particle->TotalLife;
            }
        }

        if (m_ParticleData[i].Lifetime > m_ParticleData[i].TotalLife)
        {
            --m_ActiveParticles;
            m_ParticleData[i] = m_ParticleData[m_ActiveParticles];
            --i;
        }


        if (m_ActiveParticles > 1)
            orderParticles(m_ParticleData, m_ActiveParticles);
    }
    return true;
}

float CParticleSystemInstance::GetDistanceToCamera(Vect3f particlePosition)
{
    Vect3f l_CameraDirection = CEngine::GetInstance().GetCameraController().GetFront();
    Vect3f a = (particlePosition*l_CameraDirection);
    Vect3f b = (CEngine::GetInstance().GetCameraController().GetPosition()*l_CameraDirection);

    return a.Distance(b);
}

void CParticleSystemInstance::orderParticles(ParticleData arr[], int length)
{
    int i, j;
    ParticleData aux;
    for (i = 1; i < length; ++i)
    {
        j = i;
        //while (j > 0 && arr[j-1].DistanceToCamera < arr[j].DistanceToCamera) //orden descendente
        while (j > 0 && arr[j].DistanceToCamera < arr[j-1].DistanceToCamera) //orden ascendente
        {
            aux = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = aux;
            --j;
        }
    }
}

bool CParticleSystemInstance::Render(CRenderManager& lRM)
{
    for (int i = 0; i < m_ActiveParticles; ++i)
    {
        ParticleData *particle = &m_ParticleData[i];

        m_ParticleRenderableData[i].position = particle->Position;

        float ColorControlAlpha = (particle->Lifetime < particle->NextColorControlTime) ? (particle->Lifetime - particle->LastColorControlTime) / (particle->NextColorControlTime - particle->LastColorControlTime) : 1.0f;
        m_ParticleRenderableData[i].color = particle->LastColor.Lerp(particle->NextColor, ColorControlAlpha);

        float SizeControlAlpha = (particle->Lifetime < particle->NextSizeControlTime) ? (particle->Lifetime - particle->LastSizeControlTime) / (particle->NextSizeControlTime - particle->LastSizeControlTime) : 1.0f;

        m_ParticleRenderableData[i].uv.x = mathUtils::Lerp<float>(particle->LastSize, particle->NextSize, SizeControlAlpha);
        m_ParticleRenderableData[i].uv.y = particle->Angle;
        m_ParticleRenderableData[i].uv2.x = (float)particle->CurrentFrame;
        m_ParticleRenderableData[i].uv2.y = 0;
    }

    if (m_ActiveParticles > 0)
    {
        CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
        lCBM.mFrameDesc.m_ViewProjection = lRM.GetViewProjectionMatrix();
        lCBM.mFrameDesc.m_View = lRM.GetViewMatrix();
        lCBM.mFrameDesc.m_Projection = lRM.GetProjectionMatrix();
        lCBM.mObjDesc.m_World = GetMatrix();

        lCBM.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Object);
        lCBM.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Frame);
        m_Type->Getmaterial()->Apply();

        m_Vertices->UpdateVertex(m_ParticleRenderableData, s_MaxParticlesPerInstance);
        m_Vertices->Render(lRM.GetDeviceContext(),m_ActiveParticles);
        lRM.GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);

    }

    return true;
}
