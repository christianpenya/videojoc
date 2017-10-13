#include "Material.h"
#include "TemplatedMaterialParameter.h"
#include "XML\XML.h"
#include <assert.h>
#include "Utils\EnumToString.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"
#include "Utils\CheckedDelete.h"
#include "Graphics/Textures/TextureManager.h"
#include "Engine\Engine.h"
#include "Graphics/Effects\Technique.h"
#include "Graphics/Effects/TechniquePoolManager.h"
#include "Graphics/Buffers/ConstantBufferManager.h"

#define INDEX_DIFFUSE_TEXTURE 0
#define INDEX_NORMAL_TEXTURE 1
#define INDEX_LIGHTMAP_TEXTURE 2
#define INDEX_SPECULAR_TEXTURE 3
#define INDEX_SHADOWMAP_TEXTURE 4
#define INDEX_SHADOWMASK_TEXTURE 5
#define INDEX_CUBEMAP_TEXTURE 8
#define INDEX_SPECULAR_TEXTURE 10

CMaterial::~CMaterial()
{
    base::utils::CheckedDelete(mParameters);
}

CMaterial::CMaterial(CXMLElement* aElement) : CName( aElement )
{
    CEngine &l_Engine = CEngine::GetInstance();
    CTechniquePoolManager &l_TechniquePoolManager = l_Engine.GetTechniquePoolManager();
    CTextureManager &lTextureManager = l_Engine.GetTextureManager();

    std::string lVertexType = aElement->GetAttribute<std::string>("vertex_type", "");
    std::string lEffect = aElement->GetAttribute<std::string>("effect", "");

    mTechnique = l_TechniquePoolManager(lVertexType);
    mTechnique = l_TechniquePoolManager(lEffect);
    mTechnique = strcmp(lEffect.c_str(), "") == 0 ? l_TechniquePoolManager(lVertexType) : l_TechniquePoolManager(lEffect);

    mTextures.resize(ETextureIndex::eIndexCount);

    for (tinyxml2::XMLElement *iTextureOrParameter = aElement->FirstChildElement(); iTextureOrParameter != nullptr; iTextureOrParameter = iTextureOrParameter->NextSiblingElement())
    {
        if (strcmp(iTextureOrParameter->Name(), "texture") == 0)
        {
            //leemos textura
            ETextureIndex eTextureIndex;
            EnumString<ETextureIndex>::ToEnum(eTextureIndex, iTextureOrParameter->GetAttribute<std::string>("type", ""));
            mTextures[eTextureIndex] = lTextureManager.GetTexture(iTextureOrParameter->GetAttribute<std::string>("filename", ""));
        }
        else if (strcmp(iTextureOrParameter->Name(), "parameter") == 0)
        {
            TParameterType lType;

            if (EnumString<TParameterType>::ToEnum(lType, iTextureOrParameter->GetAttribute<std::string>("type", "")))

            {
                CMaterialParameter* lParameter = nullptr;

#define CASE_CREATE_MATERIAL_PARAMETER( parameter_type, parameter_class_type, parameter_def_value ) \
case parameter_type: \
{ \
    lParameter = new CTemplatedMaterialParameter<parameter_class_type>(\
        iTextureOrParameter->GetAttribute<std::string>("name", ""), \
		iTextureOrParameter->GetAttribute<parameter_class_type>("value", parameter_def_value), \
		iTextureOrParameter->GetAttribute<float>("min_value", 0.0f), \
		iTextureOrParameter->GetAttribute<float>("max_value", 1.0f), \
        parameter_type);\
}\
break;\

                switch (lType)
                {
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat, float, 0.0f);
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat2, Vect2f, Vect2f(0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat3, Vect3f, Vect3f(0.0f, 0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat4, Vect4f, Vect4f(0.0f, 0.0f, 0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eColor, CColor, CColor(0.0f, 0.0f, 0.0f, 0.0f));

                default:
                    break;
                }

#undef CASE_CREATE_MATERIAL_PARAMETER

                if (lParameter)
                    mParameters.push_back(lParameter);
            }
        }
    }
}

void CMaterial::Apply()
{
    assert(mTechnique);
    ID3D11DeviceContext * lContext = CEngine::GetInstance().GetRenderManager().GetDeviceContext();

    // Bind the technique that will render this geometry
    mTechnique->Bind(lContext);

    for (size_t i = 0, lCount = mTextures.size(); i < lCount; ++i)
    {
        if (mTextures[i])
        {
            mTextures[i]->Bind(i, lContext);
        }
    }

    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();

    for (size_t i = 0, lCount = mParameters.size(); i < lCount; ++i)
    {
        lCBM.mMaterialDesc.m_RawData[i] = *(Vect4f *)mParameters[i]->GetAddr();
    }

    lCBM.BindBuffer(lContext, CConstantBufferManager::CB_Material);
}


void CMaterial::DrawImgui()
{
    if ((mParameters.size() > 0))
    {
        ImGui::ColorEditMode(ImGuiColorEditMode_RGB);
        if (ImGui::TreeNode(GetName().c_str()))
        {
            for (int i = 0; i < mParameters.size(); ++i)
            {
                ImGui::PushID(i);

                switch (mParameters[i]->GetType())
                {
                case eFloat:
                    ImGui::SliderFloat(mParameters[i]->GetName().c_str(), (float*)mParameters[i]->GetAddr(0), mParameters[i]->GetImGuiMin(), mParameters[i]->GetImGuiMax());
                    break;
                case eFloat2:
                    ImGui::SliderFloat2(mParameters[i]->GetName().c_str(), ((float*)mParameters[i]->GetAddr(0), (float*)mParameters[i]->GetAddr(1)), 0.0f, 1.0f, "%.2f");
                    break;
                case eFloat3:
                    ImGui::SliderFloat3(mParameters[i]->GetName().c_str(), ((float*)mParameters[i]->GetAddr(0), (float*)mParameters[i]->GetAddr(1), (float*)mParameters[i]->GetAddr(2)), 0.0f, 1.0f, "%.2f");
                    break;
                case eFloat4:
                    ImGui::SliderFloat4(mParameters[i]->GetName().c_str(), ((float*)mParameters[i]->GetAddr(0), (float*)mParameters[i]->GetAddr(1), (float*)mParameters[i]->GetAddr(2), (float*)mParameters[i]->GetAddr(4)), 0.0f, 1.0f, "%.2f");
                    break;
                case eColor:
                    ImGui::SliderFloat("Red", (float*)mParameters[i]->GetAddr(0), 0.0f, 1.0f);
                    ImGui::SliderFloat("Green", (float*)mParameters[i]->GetAddr(1), 0.0f, 1.0f);
                    ImGui::SliderFloat("Blue", (float*)mParameters[i]->GetAddr(2), 0.0f, 1.0f);
                    ImGui::SliderFloat("Alpha", (float*)mParameters[i]->GetAddr(3), 0.0f, 1.0f);
                    break;

                default:
                    ImGui::BulletText("Unexpected paramater.");
                    ImGui::BulletText(mParameters[i]->GetName().c_str());
                    break;

                }
            }
        }
    }
    /*    if (mParameters.size() > 0)
        {
            static bool show_app_auto_resize = true;
            ImGui::Begin("Menu", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

            ImGui::ColorEditMode(ImGuiColorEditMode_RGB);

            if (ImGui::CollapsingHeader(GetName().c_str(), ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::BeginChild("#Material", ImVec2(400, 200), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                ImGui::PushItemWidth(-130);

                for (size_t i = 0; i < mParameters.size(); ++i)
                {
                    ImGui::PushID(i);

                    switch (mParameters[i]->GetType())
                    {
                    case eFloat:
                    {
                        if (mParameters[i]->GetName() == "textureSize")
                        {
                            ImGui::SliderFloat(mParameters[i]->GetDescription().c_str(), (float*)mParameters[i]->GetAddr(0), 0.0f, 1024.0f);
                        }
                        else if ((mParameters[i]->GetName() == "blurScale") || (mParameters[i]->GetName() == "diffuse") || (mParameters[i]->GetName() == "num_samples"))
                        {
                            ImGui::SliderFloat(mParameters[i]->GetDescription().c_str(), (float*)mParameters[i]->GetAddr(0), 0.0f, 100.0f);
                        }
                        else
                        {

                            ImGui::SliderFloat(mParameters[i]->GetDescription().c_str(), (float*)mParameters[i]->GetAddr(0), 0.0f, 1.0f);
                        }
                    }
                    break;
                    case eFloat2:
                        if (mParameters[i]->GetName() == "fog_start_end")
                        {
                            ImGui::SliderFloat("Fog Start", (float*)mParameters[i]->GetAddr(0), 0.0f, 50.0f);
                            ImGui::SliderFloat("Fog End", (float*)mParameters[i]->GetAddr(1), 0.0f, 100.0f);
                        }
                        else if (mParameters[i]->GetName() == "specular_exponent_contrib")
                        {
                            ImGui::SliderFloat("Specular Exponent", (float*)mParameters[i]->GetAddr(0), 0.0f, 100.0f);
                            ImGui::SliderFloat("Specular Contrib", (float*)mParameters[i]->GetAddr(1), 0.0f, 1.0f);
                        }
                        else if (mParameters[i]->GetName() == "RTSize")
                        {
                            ImGui::SliderFloat("RT Size X", (float*)mParameters[i]->GetAddr(0), 0.0f, 1366.0f);
                            ImGui::SliderFloat("RT Size Y", (float*)mParameters[i]->GetAddr(1), 0.0f, 768.0f);
                        }
                        else if (mParameters[i]->GetName() == "ssr_screen_resolution")
                        {
                            ImGui::SliderFloat("Screen Resolution X", (float*)mParameters[i]->GetAddr(0), 0.0f, 1366.0f);
                            ImGui::SliderFloat("Screen Resolution Y", (float*)mParameters[i]->GetAddr(1), 0.0f, 768.0f);
                        }
                        else if (mParameters[i]->GetName() == "light_position")
                        {
                            ImGui::SliderFloat("Light Position X", (float*)mParameters[i]->GetAddr(0), 0.0f, 1366.0f);
                            ImGui::SliderFloat("Light Position Y", (float*)mParameters[i]->GetAddr(1), 0.0f, 768.0f);
                        }
                        else
                            ImGui::SliderFloat2(mParameters[i]->GetName().c_str(), ((float*)mParameters[i]->GetAddr(0), (float*)mParameters[i]->GetAddr(1)), 0.0f, 1.0f);
                        break;
                    case eFloat4:
                    {
                        if (mParameters[i]->GetName() == "ssr_opacity")
                        {
                            ImGui::SliderFloat("Opacity", (float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), *(float*)mParameters[i]->GetAddr(2), "%.1f", *(float*)mParameters[i]->GetAddr(3));
                        }
                        else if (mParameters[i]->GetName() == "ssr_offset_screen")
                        {
                            ImGui::SliderFloat("Offset Screen", (float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), *(float*)mParameters[i]->GetAddr(2), "%.2f", *(float*)mParameters[i]->GetAddr(3));
                        }
                    }
                    break;
                    case eColor:
                    {

                        ImGui::ColorEdit4(mParameters[i]->GetName().c_str(), ((float*)mParameters[i]->GetAddr(0), (float*)mParameters[i]->GetAddr(1), (float*)mParameters[i]->GetAddr(2), (float*)mParameters[i]->GetAddr(3)), false);
                    }
                    break;
                    }

                    ImGui::PopID();
                }

                ImGui::PopItemWidth();
                ImGui::EndChild();
            }

            ImGui::End();
        }*/
}
