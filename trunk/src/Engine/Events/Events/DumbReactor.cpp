#include "DumbReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

CDumbReactor::CDumbReactor() {}
CDumbReactor::~CDumbReactor() {}

void CDumbReactor::Load(CXMLElement* aElement)
{
    LOG_INFO_APPLICATION("Loading! I'm loading!");
}

void CDumbReactor::React()
{
    LOG_INFO_APPLICATION("Reacting! I'm reacting!");
}

void CDumbReactor::Update(float elapsedTime)
{
    LOG_INFO_APPLICATION("React updating!");
}