#pragma once

#ifndef _ENGINE_MATERIALMANAGER_CPB_1611241620428_H
#define _ENGINE_MATERIALMANAGER_CPB_1611241620428_H

#include "Base/XML/tinyxml2/tinyxml2.h"
#include <list>
#include "Base\Utils\TemplatedMap.h"
#include "Base\Utils\Name.h"
#include "Base\Utils\Defines.h"
#include "Material.h"
#include <assert.h>

XML_FORWARD_DECLARATION

class CMaterialManager : public base::utils::CTemplatedMap<CMaterial>
{

public:
    CMaterialManager() {};
    virtual ~CMaterialManager() {};
    void Load(const std::string &LevelFilename, const std::string &DefaultsFileName = "");
    void Reload();
    void Save();

private:
    const std::string m_materialsFolderPath = "data/materials/";

    std::string m_LevelMaterialsFilename;
    std::string m_DefaultMaterialsFilename;
    bool Load(const std::string &Filename, bool Update = false);
};

#endif //_ENGINE_MATERIALMANAGER_CPB_1611241620428_H
