#pragma once

#ifndef _ENGINE_TEMPLATEDMATERIALPARAMETER_CPB_161215_H
#define _ENGINE_TEMPLATEDMATERIALPARAMETER_CPB_161215_H

#include "MaterialParameter.h"

template<typename T>
class CTemplatedMaterialParameter : public CMaterialParameter
{
public:
    //CTemplatedMaterialParameter(const std::string& aName, const T &Value, CMaterial::TParameterType aType) : CMaterialParameter(aName, aType), m_Value(Value){};
    CTemplatedMaterialParameter(const std::string& aName, const T &Value, float ImGuiSpeed, float ImGuiMin, float ImGuiMax, CMaterial::TParameterType aType)
        : CMaterialParameter(aName, ImGuiSpeed, ImGuiMin, ImGuiMax, aType)
        , m_Value(Value)
    {}
    virtual ~CTemplatedMaterialParameter() {};
    virtual uint32_t GetSize() const
    {
        return sizeof(T);
    };
    void * GetAddr(int index = 0) const;
    GET_SET_REF(T, Value);

private:
    T m_Value;
};

template<typename T> void* CTemplatedMaterialParameter<T>::GetAddr(int index = 0) const
{
    return (void*)((float*)&m_Value + index);
}

#endif //_ENGINE_MATERIALPARAMETER_CPB_161215_H
