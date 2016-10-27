#include "Name.h"

#include "XML/XML.h"

CName::CName()
    : m_Name("")
{
}

CName::CName(const CXMLElement* TreeNode)
    : m_Name(TreeNode->GetAttribute<std::string>("name", ""))
{
}

CName::CName(const std::string& Name)
    : m_Name(Name)
{
}

CName::~CName()
{

}