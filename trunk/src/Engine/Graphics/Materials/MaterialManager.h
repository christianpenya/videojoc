#pragma once
#ifndef _ENGINE_MATERIALMANAGER_CPB_1611241620428_H
#define _ENGINE_MATERIALMANAGER_CPB_1611241620428_H

#include "Utils\TemplatedMap.h"
#include "Utils\Defines.h"
#include "Material.h"

XML_FORWARD_DECLARATION

class CMaterialManager : public base::utils::CTemplatedMap<CMaterial>
{

public:
    CMaterialManager();
    virtual ~CMaterialManager();
    void Load(const std::string &LevelFilename, const std::string &DefaultsFileName = "");
    void Reload();
    void Save();
private:
    std::string m_LevelMaterialsFilename;
    std::string m_DefaultMaterialsFilename;
    bool Load(const std::string &Filename, bool Update = false);

};

#endif //_ENGINE_MATERIALMANAGER_CPB_1611241620428_H
