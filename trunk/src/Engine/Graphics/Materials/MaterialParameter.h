#pragma once

#ifndef _ENGINE_MATERIALPARAMETER_CPB_161215_H
#define _ENGINE_MATERIALPARAMETER_CPB_161215_H

#include<string>
#include "Graphics/Materials\Material.h"
#include <stdint.h>

class CMaterialParameter : public CName
{
public:
    //CMaterialParameter(const std::string& aName, CMaterial::TParameterType aType) : CName(aName), m_Type(aType) {}; //old
    CMaterialParameter(const std::string& aName, float ImGuiSpeed, float ImGuiMin, float ImGuiMax, CMaterial::TParameterType aType)
        : CName(aName), m_Type(aType), m_ImGuiSpeed(ImGuiSpeed), m_ImGuiMin(ImGuiMin), m_ImGuiMax(ImGuiMax) {};
    virtual ~CMaterialParameter() {}
    virtual uint32_t GetSize() const = 0;
    virtual void * GetAddr(int index = 0) const = 0;
    GET_SET_REF(std::string, Description)
    GET_SET(CMaterial::TParameterType, Type)
    GET_SET_REF(float, ImGuiSpeed);
    GET_SET_REF(float, ImGuiMin);
    GET_SET_REF(float, ImGuiMax);
protected:
    std::string m_Description;
    CMaterial::TParameterType m_Type;
    float m_ImGuiSpeed;
    float m_ImGuiMin;
    float m_ImGuiMax;

private:
    DISALLOW_COPY_AND_ASSIGN(CMaterialParameter);
};

#endif //_ENGINE_MATERIALPARAMETER_CPB_161215_H
