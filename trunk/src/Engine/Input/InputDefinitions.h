#pragma once
#ifndef _ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H
#define _ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H

#include <string.h>

namespace InputDefinitions
{

enum MouseButton
{
    LEFT, MIDDLE, RIGHT
};

enum MouseAxis
{
    MOUSE_X, MOUSE_Y, DX, DY, DZ
};

enum GamepadButton
{
    A, B, X, Y, START, BACK, LEFT_THUMB, RIGHT_THUMB, LB, RB, DPAD_UP, DPAD_LEFT, DPAD_RIGHT, DPAD_DOWN
};

enum GamepadAxis
{
    LEFT_THUMB_X, LEFT_THUMB_Y, RIGHT_THUMB_X, RIGHT_THUMB_Y, LEFT_TRIGGER, RIGHT_TRIGGER
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

inline GamepadButton GetGamepadButtonFromString(const char* str, GamepadButton defaultValue = (GamepadButton)-1)
{
    if (str == nullptr)
        return defaultValue;
    else if (strcmp(str, "A") == 0)
        return A;
    else if (strcmp(str, "B") == 0)
        return B;
    else if (strcmp(str, "X") == 0)
        return X;
    else if (strcmp(str, "Y") == 0)
        return Y;
    else if (strcmp(str, "START") == 0)
        return START;
    else if (strcmp(str, "BACK") == 0)
        return BACK;
    else if (strcmp(str, "LEFT_THUMB") == 0)
        return LEFT_THUMB;
    else if (strcmp(str, "RIGHT_THUMB") == 0)
        return RIGHT_THUMB;
    else if (strcmp(str, "LB") == 0)
        return LB;
    else if (strcmp(str, "RB") == 0)
        return RB;
    else if (strcmp(str, "DPAD_UP") == 0)
        return DPAD_UP;
    else if (strcmp(str, "DPAD_RIGHT") == 0)
        return DPAD_RIGHT;
    else if (strcmp(str, "DPAD_DOWN") == 0)
        return DPAD_DOWN;
    else if (strcmp(str, "DPAD_LEFT") == 0)
        return DPAD_LEFT;
    else
        return (GamepadButton)-1;
}

inline GamepadAxis GetGamepadAxisFromString(const char* str, GamepadAxis defaultValue = (GamepadAxis)-1)
{
    if (str == nullptr)
        return defaultValue;
    else if (strcmp(str, "LEFT_THUMB_X") == 0)
        return LEFT_THUMB_X;
    else if (strcmp(str, "LEFT_THUMB_Y") == 0)
        return LEFT_THUMB_Y;
    else if (strcmp(str, "RIGHT_THUMB_X") == 0)
        return RIGHT_THUMB_X;
    else if (strcmp(str, "RIGHT_THUMB_Y") == 0)
        return RIGHT_THUMB_Y;
    else if (strcmp(str, "LEFT_TRIGGER") == 0)
        return LEFT_TRIGGER;
    else if (strcmp(str, "RIGHT_TRIGGER") == 0)
        return RIGHT_TRIGGER;
    else
        return (GamepadAxis)-1;
}
}

#endif //_ENGINE_INPUTDEFINITIONS_CPB_1611201620428_H
