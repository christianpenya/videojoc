#pragma once
#ifndef __H_ENABLE__
#define __H_ENABLE__

#include "Defines.h"

class CEnabled
{
public:
	CEnabled();
	CEnabled(bool aEnable);
	virtual ~CEnabled();

	GET_SET_REF(bool, Enabled);

protected:
	bool m_Enabled;
};

#endif // __H_NAME__