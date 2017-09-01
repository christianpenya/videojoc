#include "DumbReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

CDumbReactor::CDumbReactor() {}
CDumbReactor::~CDumbReactor() {}

void CDumbReactor::React()
{
    LOG_INFO_APPLICATION("Reacting! I'm reacting!");
}

void CDumbReactor::Update()
{
    LOG_INFO_APPLICATION("React updating!");
}