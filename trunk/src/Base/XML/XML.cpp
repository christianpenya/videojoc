#include "XML.h"
//#include "Logger/Logger.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include <string>

namespace tinyxml2
{
#define QUERY_AND_RETURN_BASE_TYPE( type, QueryFnc ) \
type lRet;                                       \
XMLError eResult = QueryFnc( name, &lRet );      \
return (eResult == XML_SUCCESS) ? lRet : defval;

template<> int XMLElement::GetAttribute<int>(const char* name, const int& defval) const
{
    QUERY_AND_RETURN_BASE_TYPE(int, QueryIntAttribute);
}

/*template<> uint32 XMLElement::GetAttribute<uint32>(const char* name, const uint32& defval) const
{
	QUERY_AND_RETURN_BASE_TYPE(uint32, QueryUint32Attribute);
}*/

template<> float XMLElement::GetAttribute<float>(const char* name, const float& defval) const
{
    QUERY_AND_RETURN_BASE_TYPE(float, QueryFloatAttribute);
}

template<> bool XMLElement::GetAttribute<bool>(const char* name, const bool& defval) const
{
    QUERY_AND_RETURN_BASE_TYPE(bool, QueryBoolAttribute);
}
#undef QUERY_AND_RETURN_BASE_TYPE

template<> std::string XMLElement::GetAttribute<std::string>(const char* name, const std::string& defval) const
{
    const char* pszRet = Attribute(name);
    return (pszRet) ? pszRet : defval;
}

template<> Vect2f XMLElement::GetAttribute<Vect2f>(const char* name, const Vect2f& defval) const
{
    Vect2f l_V2f(0.0f, 0.0f);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_V2f = defval;
    else
        sscanf_s(pszRet, "%f %f", &l_V2f.x, &l_V2f.y);

    return l_V2f;
}

template<> Vect3f XMLElement::GetAttribute<Vect3f>(const char* name, const Vect3f& defval) const
{
    Vect3f l_V3f(0.0f, 0.0f, 0.0f);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_V3f = defval;
    else
        sscanf_s(pszRet, "%f %f %f", &l_V3f.x, &l_V3f.y, &l_V3f.z);

    return l_V3f;
}

/*template<> Vect3i XMLElement::GetAttribute<Vect3i>(const char* name, const Vect3i& defval) const
{
    Vect3i l_V3i(0, 0, 0);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_V3i = defval;
    else
        sscanf_s(pszRet, "%d %d %d", &l_V3i.x, &l_V3i.y, &l_V3i.z);

    return l_V3i;
}*/

template<> Vect4f XMLElement::GetAttribute<Vect4f>(const char* name, const Vect4f& defval) const
{
    Vect4f l_V4f(0.0f, 0.0f, 0.0f, 0.0f);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_V4f = defval;
    else
        sscanf_s(pszRet, "%f %f %f %f", &l_V4f.x, &l_V4f.y, &l_V4f.z, &l_V4f.w);

    return l_V4f;
}

template<> CColor XMLElement::GetAttribute<CColor>(const char* name, const CColor& defval) const
{
    CColor l_Color(0.0f, 0.0f, 0.0f, 0.0f);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_Color = defval;
    else
        sscanf_s(pszRet, "%f %f %f %f", &l_Color.x, &l_Color.y, &l_Color.z, &l_Color.w);

    return l_Color;
}

template<> Quatf XMLElement::GetAttribute<Quatf>(const char* name, const Quatf& defval) const
{
    Quatf l_Qf(0.0f, 0.0f, 0.0f, 0.0f);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_Qf = defval;
    else
        sscanf_s(pszRet, "%f %f %f %f", &l_Qf.x, &l_Qf.y, &l_Qf.z, &l_Qf.w);

    return l_Qf;
}

template<> Vect2u XMLElement::GetAttribute<Vect2u>(const char* name, const Vect2u& defval) const
{
    Vect2u l_V2u(0, 0);
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_V2u = defval;
    else
        sscanf_s(pszRet, "%d %d", &l_V2u.x, &l_V2u.y);

    return l_V2u;
}



template<> uint32 XMLElement::GetAttribute<uint32>(const char* name, const uint32& defval) const
{
    uint32 l_uint32;
    const char* pszRet = nullptr;

    pszRet = Attribute(name);
    if (pszRet == nullptr)
        l_uint32 = defval;
    else
        sscanf_s(pszRet, "%d", &l_uint32);

    return l_uint32;
}

}

namespace base
{
namespace xml
{
bool SucceedLoad(tinyxml2::XMLError aError)
{
#define CASE_ERROR_XML( xml_error ) case tinyxml2::##xml_error: lMsg = #xml_error; break;
    bool lOk = true;
    if (aError != tinyxml2::XML_SUCCESS)
    {
        lOk = false;
        std::string lMsg = "";
        switch (aError)
        {
            CASE_ERROR_XML(XML_NO_ATTRIBUTE)
            CASE_ERROR_XML(XML_WRONG_ATTRIBUTE_TYPE)
            CASE_ERROR_XML(XML_ERROR_FILE_NOT_FOUND)
            CASE_ERROR_XML(XML_ERROR_FILE_COULD_NOT_BE_OPENED)
            CASE_ERROR_XML(XML_ERROR_FILE_READ_ERROR)
            CASE_ERROR_XML(XML_ERROR_ELEMENT_MISMATCH)
            CASE_ERROR_XML(XML_ERROR_PARSING_ELEMENT)
            CASE_ERROR_XML(XML_ERROR_PARSING_ATTRIBUTE)
            CASE_ERROR_XML(XML_ERROR_IDENTIFYING_TAG)
            CASE_ERROR_XML(XML_ERROR_PARSING_TEXT)
            CASE_ERROR_XML(XML_ERROR_PARSING_CDATA)
            CASE_ERROR_XML(XML_ERROR_PARSING_COMMENT)
            CASE_ERROR_XML(XML_ERROR_PARSING_DECLARATION)
            CASE_ERROR_XML(XML_ERROR_PARSING_UNKNOWN)
            CASE_ERROR_XML(XML_ERROR_EMPTY_DOCUMENT)
            CASE_ERROR_XML(XML_ERROR_MISMATCHED_ELEMENT)
            CASE_ERROR_XML(XML_ERROR_PARSING)
            CASE_ERROR_XML(XML_CAN_NOT_CONVERT_TEXT)
            CASE_ERROR_XML(XML_NO_TEXT_NODE)
        }
        //LOG_ERROR_APPLICATION("Error '%s' when loading xml", lMsg.c_str());
        printf("Error '%s' when loading xml", lMsg.c_str());
    }
#undef CASE_ERROR_XML
    return lOk;
}
}
}
