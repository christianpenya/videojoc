#pragma once
#ifndef _ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H
#define _ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H

#include<string.h>

namespace InputDefinitions {

	enum MouseButton {
		LEFT, MIDDLE, RIGHT
	};

	enum MouseAxis {
		MOUSE_X, MOUSE_Y, DX, DY, DZ
	};

	inline MouseButton GetMouseButtonFromString(const char* str, MouseButton defaultValue = (MouseButton)-1)
	{
		if (str == nullptr)
			return defaultValue;
		else if (strcmp(str, "LEFT") == 0)
			return LEFT;
		else if (strcmp(str, "MIDDLE") == 0)
			return MIDDLE;
		else if (strcmp(str, "RIGHT") == 0)
			return RIGHT;
		else
			return (MouseButton)-1;
	}

	inline MouseAxis GetMouseAxisFromString(const char* str, MouseAxis defaultValue = (MouseAxis)-1)
	{
		if (str == nullptr)
			return defaultValue;
		else if (strcmp(str, "MOUSE_X") == 0)
			return MOUSE_X;
		else if (strcmp(str, "MOUSE_Y") == 0)
			return MOUSE_Y;
		else if (strcmp(str, "DX") == 0)
			return DX;
		else if (strcmp(str, "DY") == 0)
			return DY;
		else if (strcmp(str, "DZ") == 0)
			return DZ;
		else
			return (MouseAxis)-1;
	}
}

#endif //_ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H
