#include "Active.h"

CActive::CActive()
    : m_Active(false)
{
}

CActive::CActive(const bool aActive)
    : m_Active(aActive)
{
}

CActive::~CActive()
{
}
