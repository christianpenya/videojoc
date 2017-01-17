#pragma once
#ifndef _ENGINE_ANIMATEDCOREMODEL_20170107
#define _ENGINE_ANIMATEDCOREMODEL_20170107


#include "Base\Utils\Name.h"
#include <vector>
#include "Base\Math\Vector3.h"

class CalCoreModel;
class CMaterial;

class CAnimatedCoreModel : public CName
{
private:
    CalCoreModel *m_CalCoreModel;
    std::vector<CMaterial *> m_Materials;
    std::string m_Path;
    Vect3f m_BSPosition;
    float m_BSRadius;
    bool LoadMesh(const std::string &Filename);
    bool LoadSkeleton(const std::string &Filename);
    bool LoadAnimation(const std::string &Name, const std::string &Filename);
public:
    CAnimatedCoreModel(const std::string& aName);
    virtual ~CAnimatedCoreModel();
    CalCoreModel *GetCoreModel();
    const std::vector<CMaterial *> & GetMaterials() const
    {
        return m_Materials;
    }
    void Load(const std::string &Path);
};

#endif //_ENGINE_ANIMATEDCOREMODEL_20170107
