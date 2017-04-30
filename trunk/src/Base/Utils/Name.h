#pragma once
#ifndef __H_NAME__20170423
#define __H_NAME__20170423

#include <string>

#include "Defines.h"

XML_FORWARD_DECLARATION

class CName
{
public:
    CName();
    CName(const CXMLElement* TreeNode);
    CName(const std::string& Name);
    virtual ~CName();

    GET_SET_REF(std::string, Name);

protected:
    std::string m_Name;
};

#endif // __H_NAME__20170423
