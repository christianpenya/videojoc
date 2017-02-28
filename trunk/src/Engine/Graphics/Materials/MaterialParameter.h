#pragma once

#ifndef _ENGINE_MATERIALPARAMETER_CPB_161215_H
#define _ENGINE_MATERIALPARAMETER_CPB_161215_H

#include<string>
#include "Graphics/Materials\Material.h"
#include <stdint.h>

class CMaterialParameter : public CName
{
public:
    CMaterialParameter(const std::string& aName, CMaterial::TParameterType aType) : CName(aName), m_Type(aType) {};
    virtual ~CMaterialParameter() {}
    virtual uint32_t GetSize() const = 0;
    virtual void * GetAddr(int index = 0) const = 0;
    GET_SET_REF(std::string, Description)
    GET_SET(CMaterial::TParameterType, Type)

protected:
    std::string m_Description;
    CMaterial::TParameterType m_Type;

private:
    DISALLOW_COPY_AND_ASSIGN(CMaterialParameter);
};

#endif //_ENGINE_MATERIALPARAMETER_CPB_161215_H
