#ifndef __H_DEFINES__
#define __H_DEFINES__

#define XML_FORWARD_DECLARATION \
namespace tinyxml2				\
{								\
	class XMLElement;			\
    class XMLDocument;          \
}								\

#define CXMLElement    tinyxml2::XMLElement
#define CXMLDocument   tinyxml2::XMLDocument
#define EXMLParseError tinyxml2::XMLError

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);   \
    void operator=(const TypeName&);

#define GET_SET_BOOL(PARAMETER_NAME) \
    void Set##PARAMETER_NAME(bool PARAMETER_NAME) \
	    { \
        m_##PARAMETER_NAME=PARAMETER_NAME; \
	    } \
    bool Is##PARAMETER_NAME() \
	    { \
        return m_##PARAMETER_NAME; \
	    }
	
#define GET_SET(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(TYPE PARAMETER_NAME) \
    { \
        m_##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    TYPE Get##PARAMETER_NAME() \
    { \
        return m_##PARAMETER_NAME; \
    }

#define GET_SET_PTR(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(TYPE##* PARAMETER_NAME) \
    { \
        m_p##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    TYPE##* Get##PARAMETER_NAME() \
    { \
        return m_p##PARAMETER_NAME; \
    }

#define GET_SET_REF(TYPE, PARAMETER_NAME) \
    void Set##PARAMETER_NAME(const TYPE &##PARAMETER_NAME) \
    { \
        m_##PARAMETER_NAME=PARAMETER_NAME; \
    } \
    const TYPE & Get##PARAMETER_NAME() const \
    { \
        return m_##PARAMETER_NAME; \
    }

#define GET_SET_ARRAY(TYPE, PARAMETER_NAME, SIZE) \
    void Set##PARAMETER_NAME(TYPE PARAMETER_NAME##[]) \
    { \
        for (int i = 0; i < SIZE; ++i)\
        {\
            m_##PARAMETER_NAME##[i] = PARAMETER_NAME##[i];\
        }\
    } \
    TYPE##* Get##PARAMETER_NAME() \
    { \
        return m_##PARAMETER_NAME; \
    }

#endif // __H_DEFINES__
