#pragma once
#ifndef __H_TEMPLATED_MAP_VECTOR__
#define __H_TEMPLATED_MAP_VECTOR__

#include <string>
#include <map>
#include <assert.h>

#include "CheckedDelete.h"

namespace base
{
namespace utils
{
template<class T>
class CTemplatedMapVector
{
public:
    class CMapResourceValue
    {
    public:
        T *m_Value;
        size_t m_Id;
        CMapResourceValue()
        {
            assert(!"Method must not be called");
        }
        CMapResourceValue(T *aValue, size_t aId) : m_Value(aValue), m_Id(aId) {}
    };

    typedef std::vector<T *> TVectorResources;
    typedef std::map<std::string, CMapResourceValue> TMapResources;

public:
    CTemplatedMapVector() {}
    virtual ~CTemplatedMapVector()
    {
        Destroy();
    }

    size_t GetCount()
    {
        return m_ResourcesVector.size();
    }

    std::vector<T *> GetResourcesVector()
    {
        return m_ResourcesVector;
    }

    bool Exist(const std::string &aName)
    {
        return m_ResourcesMap.find(aName) != m_ResourcesMap.end();
    }

    void Remove(const std::string &aName)
    {
        CMapResourceValue l_ResourceValue = m_ResourcesMap[aName];
        size_t index = l_ResourceValue.m_Id;
        CheckedDelete(m_ResourcesVector[index]);
        //CheckedDelete(m_ResourcesMap[aName]);
        m_ResourcesMap.erase(aName);
        m_ResourcesVector.erase(m_ResourcesVector.begin() + index);
        for (TMapResources::iterator lItb = m_ResourcesMap.begin(), lIte = m_ResourcesMap.end(); lItb != lIte; ++lItb)
        {
            if (lItb->second.m_Id > index)
            {
                lItb->second.m_Id--;
            }
        }
    }

    virtual T * operator[](size_t aId)
    {
        return m_ResourcesVector[aId];
    }

    virtual T * operator()(const std::string &aName)
    {
        TMapResources::const_iterator lItfind = m_ResourcesMap.find(aName);
        return (lItfind != m_ResourcesMap.end()) ? lItfind->second.m_Value : nullptr;
    }

    virtual bool Add(const std::string &aName, T *Resource)
    {
        bool lOk = false;
        if (m_ResourcesMap.find(aName) == m_ResourcesMap.end())
        {
            m_ResourcesMap.insert(std::pair<std::string, CMapResourceValue>(aName, CMapResourceValue(Resource, m_ResourcesVector.size())));
            m_ResourcesVector.push_back(Resource);
            lOk = true;
        }
        return lOk;
    }

    virtual void Clear()
    {
        m_ResourcesMap.clear();
        m_ResourcesVector.clear();
    }

    virtual void Destroy()
    {
        CheckedDelete(m_ResourcesVector);
        Clear();
    }



protected:
    TVectorResources m_ResourcesVector;
    TMapResources	 m_ResourcesMap;
};
}
}

#endif // __H_TEMPLATED_MAP_VECTOR__