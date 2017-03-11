#pragma once
#ifndef _ENGINE_INPUTMANAGER_CPB_1611201620428_H
#define _ENGINE_INPUTMANAGER_CPB_1611201620428_H

#include "InputDefinitions.h"
#include <Windows.h>
#include <Xinput.h>

class CInputManager
{

public:

    CInputManager(HWND hWnd)
    {
        SetupHighprecisionMouse(hWnd);
        SetupGamepad();
        InitializeKeyboard();
    }
    virtual ~CInputManager() {}

    bool HandleKeyboard(const MSG& msg);
    bool HandleMouse(const MSG& msg);

    void PreUpdate(bool isWindowActive);
    void PostUpdate();

    void SetupHighprecisionMouse(HWND hWnd);

    // ----

    bool IsKeyPressed(unsigned char KeyCode) const
    {
        return m_KeyboardState.raw[KeyCode];
    }
    bool KeyBecomesPressed(unsigned char KeyCode) const
    {
        return m_KeyboardState.raw[KeyCode] && !m_PreviousKeyboardState.raw[KeyCode];
    }
    bool KeyBecomesReleased(unsigned char KeyCode) const
    {
        return !m_KeyboardState.raw[KeyCode] && m_PreviousKeyboardState.raw[KeyCode];
    }

    /*    bool IsEscapePressed(unsigned char KeyCode) const
        {
            return m_KeyboardState.escape;
        }
        bool EscapeBecomesPressed(unsigned char KeyCode) const
        {
            return m_KeyboardState.escape && !m_PreviousKeyboardState.escape;
        }
        bool EscapeBecomesReleased(unsigned char KeyCode) const
        {
            return !m_KeyboardState.escape && m_PreviousKeyboardState.escape;
        }

        bool IsSpacePressed(unsigned char KeyCode) const
        {
            return m_KeyboardState.space;
        }
        bool SpaceBecomesPressed(unsigned char KeyCode) const
        {
            return m_KeyboardState.space && !m_PreviousKeyboardState.space;
        }
        bool SpaceBecomesReleased(unsigned char KeyCode) const
        {
            return !m_KeyboardState.space && m_PreviousKeyboardState.space;
        }
    	*/
    float GetMouseMovementX() const
    {
        return m_MouseMovementX;
    }
    float GetMouseMovementY() const
    {
        return m_MouseMovementY;
    }
    float GetMouseMovementZ() const
    {
        return m_MouseMovementZ;
    }

    float GetMouseX() const
    {
        return m_MouseX;
    }
    float GetMouseY() const
    {
        return m_MouseY;
    }

    float GetMouseAxis(InputDefinitions::MouseAxis axis) const;

    bool IsLeftMouseButtonPressed() const
    {
        return m_ButtonLeft;
    }
    bool IsMiddleMouseButtonPressed() const
    {
        return m_ButtonMiddle;
    }
    bool IsRightMouseButtonPressed() const
    {
        return m_ButtonRight;
    }

    bool LeftMouseButtonBecomesPressed() const
    {
        return m_ButtonLeft && !m_PreviousButtonLeft;
    }
    bool MiddleMouseButtonBecomesPressed() const
    {
        return m_ButtonMiddle && !m_PreviousButtonMiddle;
    }
    bool RightMouseButtonBecomesPressed() const
    {
        return m_ButtonRight && !m_PreviousButtonRight;
    }

    bool LeftMouseButtonBecomesReleased() const
    {
        return !m_ButtonLeft && m_PreviousButtonLeft;
    }
    bool MiddleMouseButtonBecomesReleased() const
    {
        return !m_ButtonMiddle && m_PreviousButtonMiddle;
    }
    bool RightMouseButtonBecomesReleased() const
    {
        return !m_ButtonRight && m_PreviousButtonRight;
    }

    bool IsMouseButtonPressed(InputDefinitions::MouseButton button) const;
    bool MouseButtonBecomesPressed(InputDefinitions::MouseButton button) const;
    bool MouseButtonBecomesReleased(InputDefinitions::MouseButton button) const;

    void SetupGamepad();
    bool HasGamepad() const
    {
        return m_HasGamepad;
    }

    bool IsGamepadButtonPressed(InputDefinitions::GamepadButton button) const;
    bool GamepadButtonBecomesPressed(InputDefinitions::GamepadButton button) const;
    bool GamepadButtonBecomesReleased(InputDefinitions::GamepadButton button) const;
    float GetGamepadAxis(InputDefinitions::GamepadAxis axis) const;

    /*float GetGamepadLeftThumbX() const { return m_GamepadStickLeftX; }
    float GetGamepadLeftThumbY() const { return m_GamepadStickLeftY; }
    float GetGamepadRightThumbX() const { return m_GamepadStickRightX; }
    float GetGamepadRightThumbY() const { return m_GamepadStickRightY; }
    float GetGamepadLeftTrigger() const { return m_GamepadTriggerLeft; }
    float GetGamepadRightTrigger() const { return m_GamepadTriggerRight; }

    bool IsGamepadAPressed() const { return m_GamepadA; }
    bool IsGamepadBPressed() const { return m_GamepadB; }
    bool IsGamepadXPressed() const { return m_GamepadX; }
    bool IsGamepadYPressed() const { return m_GamepadY; }*/

    inline float ProcessXInputStickValue(SHORT Value, SHORT DeadZoneThreshold)
    {

        float Result = 0;

        if (Value < -DeadZoneThreshold)
        {
            Result = (float)((Value + DeadZoneThreshold) / (32768.0f - DeadZoneThreshold));
        }
        else if (Value > DeadZoneThreshold)
        {
            Result = (float)((Value - DeadZoneThreshold) / (32768.0f - DeadZoneThreshold));
        }

        return Result;
    }

    inline float ProcessXInputTriggerValue(SHORT Value, SHORT DeadZoneThreshold)
    {

        float Result = 0;

        if (Value > DeadZoneThreshold)
        {
            Result = (float)((Value - DeadZoneThreshold) / (255.0f - DeadZoneThreshold));
        }

        return Result;
    }

private:

    bool UpdateKeyboard();
    bool UpdateMouse();

    struct KeyboardData
    {
        bool raw[256];
        bool escape = false, space = false;
        bool numpad[10];
        bool fkey[24];
        bool left = false, right = false, up = false, down = false;

        void SetKey(unsigned char key, bool state);

    } m_PreviousKeyboardState, m_KeyboardState;

    void InitializeKeyboard()
    {
        for (int i = 0; i < 256; ++i)
        {
            m_PreviousKeyboardState.raw[i] = false;
            m_KeyboardState.raw[i] = false;
        }

        for (int i = 0; i < 10; ++i)
        {
            m_PreviousKeyboardState.numpad[i] = false;
            m_KeyboardState.numpad[i] = false;
        }

        for (int i = 0; i < 24; ++i)
        {
            m_PreviousKeyboardState.fkey[i] = false;
            m_KeyboardState.fkey[i] = false;
        }

    }

    bool m_HighPrecisionMouseRegistered;

    float								m_MouseMovementX = 0.0f;
    float								m_MouseMovementY = 0.0f;
    float								m_MouseMovementZ = 0.0f;
    float								m_MouseX = 0.0f;
    float								m_MouseY = 0.0f;
    bool								m_ButtonRight = false, m_PreviousButtonRight = false;
    bool								m_ButtonLeft = false, m_PreviousButtonLeft = false;
    bool								m_ButtonMiddle = false, m_PreviousButtonMiddle = false;

    bool								m_HasGamepad;

    float								m_GamepadStickLeftX = 0.0f, m_GamepadStickLeftY = 0.0f;
    float								m_GamepadStickRightX = 0.0f, m_GamepadStickRightY = 0.0f;
    float								m_GamepadTriggerLeft = 0.0f, m_GamepadTriggerRight = 0.0f;

    bool								m_GamepadLeftThumbButton = false, m_PreviousGamepadLeftThumbButton = false;
    bool								m_GamepadRightThumbButton = false, m_PreviousGamepadRightThumbButton = false;
    bool								m_GamepadA = false, m_PreviousGamepadA = false;
    bool								m_GamepadB = false, m_PreviousGamepadB = false;
    bool								m_GamepadX = false, m_PreviousGamepadX = false;
    bool								m_GamepadY = false, m_PreviousGamepadY = false;
    bool								m_GamepadRight = false, m_PreviousGamepadRight = false;
    bool								m_GamepadLeft = false, m_PreviousGamepadLeft = false;
    bool								m_GamepadSelect = false;
    bool								m_GamepadStart = false;
    bool								m_GamepadDpadUp = false, m_PreviousGamepadDpadUp = false;
    bool								m_GamepadDpadRight = false, m_PreviousGamepadDpadRight = false;
    bool								m_GamepadDpadDown = false, m_PreviousGamepadDpadDown = false;
    bool								m_GamepadDpadLeft = false, m_PreviousGamepadDpadLeft = false;
};

#endif //_ENGINE_INPUTMANAGER_CPB_1611201620428_H
