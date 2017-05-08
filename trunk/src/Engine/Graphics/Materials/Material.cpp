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

CMaterial::~CMaterial()
{
    /*
    for (std::vector< CMaterialParameter *>::iterator iMaterialParameter = mParameters.begin();
                iMaterialParameter != mParameters.end();
                ++iMaterialParameter)
    {}
    */
    base::utils::CheckedDelete(mParameters);
}

CMaterial::CMaterial(CXMLElement* aElement) : CName( aElement )
{
    CEngine &l_Engine = CEngine::GetInstance();
    CTechniquePoolManager &l_TechniquePoolManager = l_Engine.GetTechniquePoolManager();

    std::string lVertexType = aElement->GetAttribute<std::string>("vertex_type", "");
    std::string lEffect = aElement->GetAttribute<std::string>("effect", "");

    mTechnique = l_TechniquePoolManager(lVertexType);
    mTechnique = l_TechniquePoolManager(lEffect);


    mTechnique = strcmp(lEffect.c_str(), "") == 0 ? l_TechniquePoolManager(lVertexType) : l_TechniquePoolManager(lEffect);

    for (tinyxml2::XMLElement *iTextureOrParameter = aElement->FirstChildElement(); iTextureOrParameter != nullptr; iTextureOrParameter = iTextureOrParameter->NextSiblingElement())
    {
        if (strcmp(iTextureOrParameter->Name(), "texture") == 0)
        {
            //leemos textura
            std::string textureName = iTextureOrParameter->GetAttribute<std::string>("type", "");
            std::string textureFilename = iTextureOrParameter->GetAttribute<std::string>("filename", "");

            CTextureManager &lTextureManager = l_Engine.GetTextureManager();
            CTexture * lTexture = lTextureManager.GetTexture(textureFilename);
            mTextures.push_back(lTexture);
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

    int l_pos=-1;
    int l_posFloat2 = -1;
    int l_posColor = -1;
    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
    for (size_t i = 0, lCount = mParameters.size(); i < lCount; ++i)
    {

        /*****************************************************************************
        SWITCH COLOCANDO PROPIEDADES DE MATERIALES EN EL CONSTANT BUFFER
        Cuidado con colocar dos propiedades que puedan coexistir en un mismo material
        en la misma casilla del vector!
        Posiciones ocupadas del vector:
        [0] -> COLOR DIFUSO
        [1] -> BUMP, FOG
        [2] -> REBOTES DE LUZ ESPECULAR
        *****************************************************************************/

        switch (mParameters[i]->GetType())
        {
        case eFloat:
        {
            if (mParameters[i]->GetName() == "bump")
            {
                mParameters[i]->SetDescription("Bump");
                l_pos = 1;
            }//INICIO PARAMETROS BLOOM
            else if (mParameters[i]->GetName() == "threshold")
            {
                mParameters[i]->SetDescription("Threshold");
                l_pos = 4;
            }
            else if (mParameters[i]->GetName() == "bloomActive")
            {
                mParameters[i]->SetDescription("Enabled");
                l_pos = 5;
            }
            else if (mParameters[i]->GetName() == "bloomIntensity")
            {
                mParameters[i]->SetDescription("Bloom Intensity");
                l_pos = 6;
            }
            else if (mParameters[i]->GetName() == "originalIntensity")
            {
                mParameters[i]->SetDescription("Original Intensity");
                l_pos = 7;
            }
            else if (mParameters[i]->GetName() == "bloomSaturation")
            {
                mParameters[i]->SetDescription("Bloom Saturation");
                l_pos = 8;
            }
            else if (mParameters[i]->GetName() == "originalSaturation")
            {
                mParameters[i]->SetDescription("Original Saturation");
                l_pos = 9;
            } //FIN PARAMETROS BLOOM
            //INICIO PARAMETROS COLOR GRADING
            else if (mParameters[i]->GetName() == "enabled")
            {
                mParameters[i]->SetDescription("Enabled");
                l_pos = 4;
            }
            else if (mParameters[i]->GetName() == "brownSepiaColorGrading")
            {
                mParameters[i]->SetDescription("Brown Sepia");
                l_pos = 5;
            }
            else if (mParameters[i]->GetName() == "greenSepiaColorGrading")
            {
                mParameters[i]->SetDescription("Green Sepia");
                l_pos = 6;
            }
            else if (mParameters[i]->GetName() == "blueSepiaColorGrading")
            {
                mParameters[i]->SetDescription("Blue Sepia");
                l_pos = 7;
            }
            else if (mParameters[i]->GetName() == "monochromeColorGrading")
            {
                mParameters[i]->SetDescription("Monochrome");
                l_pos = 8;
            }
            else if (mParameters[i]->GetName() == "colorColorGrading")
            {
                mParameters[i]->SetDescription("Color");
                l_pos = 9;
            }
            else if (mParameters[i]->GetName() == "contrastColorGrading")
            {
                mParameters[i]->SetDescription("Contrast");
                l_pos = 10;
            }
            else if (mParameters[i]->GetName() == "brightnessColorGrading")
            {
                mParameters[i]->SetDescription("Brightness");
                l_pos = 11;
            }//FIN PARAMETERS COLOR GRADING
            //INICIO PARAMETERS GAUSSIAN
            else if (mParameters[i]->GetName() == "textureSize")
            {
                mParameters[i]->SetDescription("Texture Size");
                l_pos = 12;
            }
            else if (mParameters[i]->GetName() == "blurScale")
            {
                mParameters[i]->SetDescription("Blur Scale");
                l_pos = 13;
            }//FIN PARAMETERS GAUSSIAN
            //INICIO PARAMETERS HDAO
            else if (mParameters[i]->GetName() == "HDAOActive")
            {
                mParameters[i]->SetDescription("Enabled");
                l_pos = 4;
            }
            else if (mParameters[i]->GetName() == "HDAOShowNormals")
            {
                mParameters[i]->SetDescription("Show Normals");
                l_pos = 5;
            }
            else if (mParameters[i]->GetName() == "HDAOShowAO")
            {
                mParameters[i]->SetDescription("Show AO");
                l_pos = 6;
            }
            else if (mParameters[i]->GetName() == "HDAOUseNormals")
            {
                mParameters[i]->SetDescription("Use Normals");
                l_pos = 7;
            }
            else if (mParameters[i]->GetName() == "AcceptAngle")
            {
                mParameters[i]->SetDescription("Accept Angle");
                l_pos = 9;
            }
            else if (mParameters[i]->GetName() == "HDAOIntensity")
            {
                mParameters[i]->SetDescription("Intensity");
                l_pos = 10;
            }
            else if (mParameters[i]->GetName() == "NormalScale")
            {
                mParameters[i]->SetDescription("Normal Scale");
                l_pos = 11;
            }
            else if (mParameters[i]->GetName() == "HDAORejectRadius")
            {
                mParameters[i]->SetDescription("Reject Radius");
                l_pos = 12;
            }
            else if (mParameters[i]->GetName() == "HDAOAcceptRadius")
            {
                mParameters[i]->SetDescription("AcceptRadius");
                l_pos = 13;
            }
            //FIN PARAMETERS HDAO
            //INICIO PARAMETROS NOISE
            else if (mParameters[i]->GetName() == "noisePct")
            {
                mParameters[i]->SetDescription("Noise Pct");
                l_pos = 4;
            }
            else if (mParameters[i]->GetName() == "vignettingPct")
            {
                mParameters[i]->SetDescription("Vignetting Pct");
                l_pos = 5;
            }
            else if (mParameters[i]->GetName() == "noiseAmount")
            {
                mParameters[i]->SetDescription("Noise Amount");
                l_pos = 6;
            }//FIN PARAMETROS NOISE
            //INICIO PARAMETROS ZBLUR
            else if (mParameters[i]->GetName() == "ZBlurActive")
            {
                mParameters[i]->SetDescription("Enabled");
                l_pos = 4;
            }
            else if (mParameters[i]->GetName() == "ZBlurShowDepths")
            {
                mParameters[i]->SetDescription("Show Depths");
                l_pos = 5;
            }
            else if (mParameters[i]->GetName() == "ZBlurConstantBlur")
            {
                mParameters[i]->SetDescription("Constant Blur");
                l_pos = 6;
            }
            else if (mParameters[i]->GetName() == "ZBlurFocalStart")
            {
                mParameters[i]->SetDescription("Focal Start");
                l_pos = 7;
            }
            else if (mParameters[i]->GetName() == "ZBlurFocalEnd")
            {
                mParameters[i]->SetDescription("Focal End");
                l_pos = 8;
            }
            else if (mParameters[i]->GetName() == "ZBlurEnd")
            {
                mParameters[i]->SetDescription("ZBlur End");
                l_pos = 9;
            }//FIN PARAMETROS ZBLUR
            //INICIO PARAMETROS SSR
            else if (mParameters[i]->GetName() == "ssr_enabled")
            {
                mParameters[i]->SetDescription("Enabled");
                l_pos = 4;
            }
            //FIN PARAMETROS SSR
            else
            {
                mParameters[i]->SetDescription(mParameters[i]->GetName());
                l_pos = 4;
            }
        }
        break;
        case eFloat2:
            if (mParameters[i]->GetName() == "fog_start_end")
                l_posFloat2 = 1;
            else if (mParameters[i]->GetName() == "specular_exponent_contrib")
                l_posFloat2 = 2;
            //INICIO PARAMETROS HDAO
            else if (mParameters[i]->GetName() == "RTSize")
                l_posFloat2 = 8;
            //FIN PARAMETROS HDAO
            //INICIO PARAMETROS SSR
            else if (mParameters[i]->GetName() == "ssr_screen_resolution")
                l_posFloat2 = 7;
            //FIN PARAMETROS SSR
            break;
        case eFloat3:
            break;
        case eFloat4:
            //INICIO PARAMETROS SSR
            if (mParameters[i]->GetName() == "ssr_opacity")
            {
                mParameters[i]->SetDescription("Opacity");
                lCBM.mMaterialDesc.m_RawData[5] = Vect4f(*(float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), *(float*)mParameters[i]->GetAddr(2), *(float*)mParameters[i]->GetAddr(3));
            }
            else if (mParameters[i]->GetName() == "ssr_offset_screen")
            {
                mParameters[i]->SetDescription("Offset Screen");
                lCBM.mMaterialDesc.m_RawData[6] = Vect4f(*(float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), *(float*)mParameters[i]->GetAddr(2), *(float*)mParameters[i]->GetAddr(3));
            }
            break;
        case eColor:
        {
            mParameters[i]->SetDescription(mParameters[i]->GetName());
            l_posColor = 0;
            lCBM.mMaterialDesc.m_RawData[l_posColor] = Vect4f(*(float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), *(float*)mParameters[i]->GetAddr(2), *(float*)mParameters[i]->GetAddr(3));
        }
        break;
        }

        if (l_pos>=0)
            lCBM.mMaterialDesc.m_RawData[l_pos] = Vect4f(*(float*)mParameters[i]->GetAddr(0), 0.0f, 0.0f, 0.0f);
        else if (l_posFloat2>=0)
            lCBM.mMaterialDesc.m_RawData[l_posFloat2] = Vect4f(*(float*)mParameters[i]->GetAddr(0), *(float*)mParameters[i]->GetAddr(1), 0.0f, 0.0f);

        l_pos = -1;
        l_posFloat2 = -1;
    }
    lCBM.BindBuffer(lContext, CConstantBufferManager::CB_Material);
}


void CMaterial::DrawImgui()
{
    //  if ((mParameters.size() > 0))
//   {

    //  static bool show_app_auto_resize = true;
    //  ImGui::Begin("Materiales", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);


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
                else if ((mParameters[i]->GetName() == "blurScale") || (mParameters[i]->GetName() == "diffuse"))
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
                    ImGui::SliderFloat("RT Size X", (float*)mParameters[i]->GetAddr(0), 0.0f, 512.0f);
                    ImGui::SliderFloat("RT Size Y", (float*)mParameters[i]->GetAddr(1), 0.0f, 512.0f);
                }
                else if (mParameters[i]->GetName() == "ssr_screen_resolution")
                {
                    ImGui::SliderFloat("Screen Resolution X", (float*)mParameters[i]->GetAddr(0), 0.0f, 1024.0f);
                    ImGui::SliderFloat("Screen Resolution Y", (float*)mParameters[i]->GetAddr(1), 0.0f, 768.0f);
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
//    ImGui::End();

//    }

}

