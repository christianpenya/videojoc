#pragma once
#ifndef _ENGINE_CINEMATIC_MANAGER_20170213
#define _ENGINE_CINEMATIC_MANAGER_20170213

#include "XML/XML.h"
#include "XML\tinyxml2\tinyxml2.h"
#include <list>
#include "Cinematic.h"
#include "Utils\TemplatedMapVector.h"
#include "Utils\Name.h"
#include "Utils\Defines.h"
#include <assert.h>

XML_FORWARD_DECLARATION

class CCinematicManager : public base::utils::CTemplatedMapVector<CCinematic>
{
public:
    CCinematicManager();
    virtual ~CCinematicManager();
    bool Load(const std::string &Filename);
    bool Load();
    bool Reload();
    bool Update(float elapsedTime);
    bool Play(const std::string &Name);
protected:
    std::string m_Filename;
};


#endif //_ENGINE_CINEMATIC_20170213