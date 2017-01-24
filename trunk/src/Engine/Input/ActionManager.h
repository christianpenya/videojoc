#pragma once
#ifndef _ENGINE_ACTIONMANAGER_CPB_1611201620428_H
#define _ENGINE_ACTIONMANAGER_CPB_1611201620428_H

#include <vector>
#include "Utils\TemplatedMap.h"
#include "InputDefinitions.h"
#include "InputManager.h"
#include "XML/XML.h"

struct ActionTrigger
{

    enum ButtonActionType
    {
        IsPressed, IsReleased, BecomesPressed, BecomesReleased
    };

    enum TriggerType
    {
        KEYBOARD,
        MOUSE,
        MOUSE_BUTTON,
        GAMEPAD,
        GAMEPAD_BUTTON
    } type;
    union
    {
        struct Keyboard
        {
            ButtonActionType actionType;
            unsigned char keyCode;
            float value;
        } keyboard;
        struct MouseButton
        {
            ButtonActionType actionType;
            InputDefinitions::MouseButton button;
            float value;
        } mouseButton;
        struct Mouse
        {
            float threshold;
            InputDefinitions::MouseAxis axis;
            bool geThreshold;
        } mouse;
        struct GamepadButton
        {
            ButtonActionType actionType;
            InputDefinitions::GamepadButton button;
            float value;
        } gamepadButton;
        struct Gamepad
        {
            float threshold;
            InputDefinitions::GamepadAxis axis;
            bool geThreshold;
        } gamepad;
    };

    static TriggerType GetTriggerTypeFromString(const char* str, TriggerType defaultValue = (TriggerType)-1);
    static ButtonActionType GetButtonActionTypeFromString(const char* str, ButtonActionType defaultValue = (ButtonActionType)-1);
};

struct InputAction
{
    float value;
    bool active;

    std::vector<ActionTrigger> triggers;
};

class CActionManager : public base::utils::CTemplatedMap < InputAction >
{

public:
    CActionManager(const CInputManager &inputManager);
    virtual ~CActionManager() {};

    bool LoadActions(const std::string &path);
    void Update();

private:
    const CInputManager& m_InputManager;
};

#endif //_ENGINE_ACTIONMANAGER_CPB_1611201620428_H
