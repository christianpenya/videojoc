#include "InputManager.h"

typedef DWORD WINAPI TInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD WINAPI TInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);

DWORD WINAPI InputGetStateStub(DWORD dwUserIndex, XINPUT_STATE *pState)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI InputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION *pState)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

static TInputGetState* InputGetState = InputGetStateStub;
static TInputSetState* InputSetState = InputSetStateStub;

void CInputManager::PreUpdate(bool isWindowActive)
{

    m_PreviousKeyboardState = m_KeyboardState;

    m_PreviousButtonLeft = m_ButtonLeft;
    m_PreviousButtonMiddle = m_ButtonMiddle;
    m_PreviousButtonRight = m_ButtonRight;

    m_MouseMovementX = m_MouseMovementY = m_MouseMovementZ = 0;

    if (!isWindowActive)
    {
        m_KeyboardState = {};
        m_ButtonLeft = m_ButtonMiddle = m_ButtonRight = false;
    }
}

void CInputManager::PostUpdate()
{

    XINPUT_STATE l_ControllerState;

    if (InputGetState(0, &l_ControllerState) == ERROR_SUCCESS)
    {
        m_HasGamepad = true;

        m_GamepadStickLeftX = ProcessXInputStickValue(l_ControllerState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        m_GamepadStickLeftY = ProcessXInputStickValue(l_ControllerState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        m_PreviousGamepadLeftThumbButton = m_GamepadLeftThumbButton;
        m_GamepadLeftThumbButton = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)!= 0;

        m_GamepadStickRightX = ProcessXInputStickValue(l_ControllerState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        m_GamepadStickRightY = ProcessXInputStickValue(l_ControllerState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        m_PreviousGamepadRightThumbButton = m_GamepadRightThumbButton;
        m_GamepadRightThumbButton = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;

        m_GamepadTriggerLeft = ProcessXInputTriggerValue(l_ControllerState.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
        m_GamepadTriggerRight = ProcessXInputTriggerValue(l_ControllerState.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

        m_PreviousGamepadA = m_GamepadA;
        m_GamepadA = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
        m_PreviousGamepadB = m_GamepadB;
        m_GamepadB = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
        m_PreviousGamepadX = m_GamepadX;
        m_GamepadX = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
        m_PreviousGamepadY = m_GamepadY;
        m_GamepadY = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

        m_PreviousGamepadLeft = m_GamepadLeft;
        m_GamepadLeft = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
        m_PreviousGamepadRight = m_GamepadRight;
        m_GamepadRight = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        m_GamepadSelect = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
        m_GamepadStart = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;

        m_PreviousGamepadDpadUp = m_GamepadDpadUp;
        m_GamepadDpadUp = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
        m_PreviousGamepadDpadRight = m_GamepadDpadRight;
        m_GamepadDpadRight = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
        m_PreviousGamepadDpadDown = m_GamepadDpadDown;
        m_GamepadDpadDown = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
        m_PreviousGamepadDpadLeft = m_GamepadDpadLeft;
        m_GamepadDpadLeft = (l_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    }
    else
    {
        m_HasGamepad = false;
    }
}

bool CInputManager::HandleKeyboard(const MSG& msg)
{

    bool handled = false;

    unsigned char VKCode = (unsigned char)msg.wParam;

    int repeat = msg.lParam & 0xffff;
    bool AltKeyWasDown = ((msg.lParam & (1 << 29)) != 0);
    bool WasDown = ((msg.lParam & (1 << 30)) != 0);

    bool IsDown = ((msg.lParam & (1 << 31)) == 0);

    if (WasDown != IsDown)
    {
        m_KeyboardState.SetKey(VKCode, IsDown);
        handled = true;
    }

    return handled;
}

bool CInputManager::HandleMouse(const MSG& msg)
{

    switch (msg.message)
    {

    case WM_LBUTTONDOWN:
        m_ButtonLeft = true;
        return true;

    case WM_MBUTTONDOWN:
        m_ButtonMiddle = true;
        return true;

    case WM_RBUTTONDOWN:
        m_ButtonRight = true;
        return true;

    case WM_LBUTTONUP:
        m_ButtonLeft = false;
        return true;

    case WM_MBUTTONUP:
        m_ButtonMiddle = false;
        return true;

    case WM_RBUTTONUP:
        m_ButtonRight = false;
        return true;

    case WM_MOUSEMOVE:
        m_MouseX = LOWORD(msg.lParam);
        m_MouseY = HIWORD(msg.lParam);
        return true;

    case WM_MOUSEWHEEL:
        m_MouseMovementZ = GET_WHEEL_DELTA_WPARAM(msg.wParam);
        return true;

    case WM_INPUT:
    {
        UINT dwSize = 60;
        static BYTE lpb[60];

        UINT read = GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)lpb;

        if (raw->header.dwType == RIM_TYPEMOUSE && read <= dwSize)
        {
            m_MouseMovementX += raw->data.mouse.lLastX;
            m_MouseMovementY += raw->data.mouse.lLastY;

            return true;
        }

        return false;
    }

    default:
        return false;
    }
}

void CInputManager::KeyboardData::SetKey(unsigned char key, bool state)
{

    raw[key] = state;

    if (key >= VK_NUMPAD0 && key <= VK_NUMPAD9)
    {
        numpad[key - VK_NUMPAD0] = state;
    }
    else if (key >= VK_F1 && key <= VK_F24)
    {
        fkey[key - VK_F1] = state;
    }
    else
    {
        switch (key)
        {
        case VK_ESCAPE:
            escape = state;
            break;
        case VK_SPACE:
            space = state;
            break;
        case VK_LEFT:
            left = state;
            break;
        case VK_UP:
            up = state;
            break;
        case VK_RIGHT:
            right = state;
            break;
        case VK_DOWN:
            down = state;
            break;
        }
    }
}

void CInputManager::SetupHighprecisionMouse(HWND hWnd)
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;
    m_HighPrecisionMouseRegistered = (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) != 0);

}

float CInputManager::GetMouseAxis(InputDefinitions::MouseAxis axis) const
{

    switch (axis)
    {

    case InputDefinitions::MOUSE_X:
        return m_MouseX;

    case InputDefinitions::MOUSE_Y:
        return m_MouseY;

    case InputDefinitions::DX:
        return m_MouseMovementX;

    case InputDefinitions::DY:
        return m_MouseMovementY;

    case InputDefinitions::DZ:
        return m_MouseMovementZ;

    default:
        assert(false);
        return 0;
    }
}

bool CInputManager::IsMouseButtonPressed(InputDefinitions::MouseButton button) const
{
    switch (button)
    {

    case InputDefinitions::MouseButton::LEFT:
        return m_ButtonLeft;

    case InputDefinitions::MouseButton::RIGHT:
        return m_ButtonRight;

    case InputDefinitions::MouseButton::MIDDLE:
        return m_ButtonMiddle;

    default:
        assert(false);
        return 0;
    }
}

bool CInputManager::MouseButtonBecomesPressed(InputDefinitions::MouseButton button) const
{

    switch (button)
    {

    case InputDefinitions::MouseButton::LEFT:
        return m_ButtonLeft && !m_PreviousButtonLeft;

    case InputDefinitions::MouseButton::RIGHT:
        return m_ButtonRight && !m_PreviousButtonRight;

    case InputDefinitions::MouseButton::MIDDLE:
        return m_ButtonMiddle && !m_PreviousButtonMiddle;

    default:
        assert(false);
        return 0;
    }
}

bool CInputManager::MouseButtonBecomesReleased(InputDefinitions::MouseButton button) const
{

    switch (button)
    {

    case InputDefinitions::MouseButton::LEFT:
        return !m_ButtonLeft && m_PreviousButtonLeft;

    case InputDefinitions::MouseButton::RIGHT:
        return !m_ButtonRight && m_PreviousButtonRight;

    case InputDefinitions::MouseButton::MIDDLE:
        return !m_ButtonMiddle && m_PreviousButtonMiddle;;

    default:
        assert(false);
        return 0;
    }
}

void CInputManager::SetupGamepad()
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary)
    {
        XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
    }

    if (!XInputLibrary)
    {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary)
    {
        InputGetState = (TInputGetState *)GetProcAddress(XInputLibrary, "XInputGetState");
        if (!InputGetState)
        {
            InputGetState = InputGetStateStub;
        }

        InputSetState = (TInputSetState *)GetProcAddress(XInputLibrary, "XInputSetState");
        if (!InputSetState)
        {
            InputSetState = InputSetStateStub;
        }
    }
}

bool CInputManager::IsGamepadButtonPressed(InputDefinitions::GamepadButton button) const
{

    switch (button)
    {

    case InputDefinitions::GamepadButton::A:
        return m_GamepadA;

    case InputDefinitions::GamepadButton::B:
        return m_GamepadB;

    case InputDefinitions::GamepadButton::X:
        return m_GamepadX;

    case InputDefinitions::GamepadButton::Y:
        return m_GamepadY;

    case InputDefinitions::GamepadButton::BACK:
        return m_GamepadSelect;

    case InputDefinitions::GamepadButton::START:
        return m_GamepadStart;

    case InputDefinitions::GamepadButton::RB:
        return m_GamepadRight;

    case InputDefinitions::GamepadButton::LB:
        return m_GamepadLeft;

    case InputDefinitions::GamepadButton::RIGHT_THUMB:
        return m_GamepadRightThumbButton;

    case InputDefinitions::GamepadButton::LEFT_THUMB:
        return m_GamepadLeftThumbButton;

    case InputDefinitions::GamepadButton::DPAD_UP:
        return m_GamepadDpadUp;

    case InputDefinitions::GamepadButton::DPAD_RIGHT:
        return m_GamepadDpadRight;

    case InputDefinitions::GamepadButton::DPAD_DOWN:
        return m_GamepadDpadDown;

    case InputDefinitions::GamepadButton::DPAD_LEFT:
        return m_GamepadDpadLeft;

    default:
        assert(false);
        return 0;
    }
}

bool CInputManager::GamepadButtonBecomesPressed(InputDefinitions::GamepadButton button) const
{

    switch (button)
    {

    case InputDefinitions::GamepadButton::A:
        return m_GamepadA && !m_PreviousGamepadA;

    case InputDefinitions::GamepadButton::B:
        return m_GamepadB && !m_PreviousGamepadB;

    case InputDefinitions::GamepadButton::X:
        return m_GamepadX && !m_PreviousGamepadX;

    case InputDefinitions::GamepadButton::Y:
        return m_GamepadY && !m_PreviousGamepadY;

    case InputDefinitions::GamepadButton::RB:
        return m_GamepadRight && !m_PreviousGamepadRight;

    case InputDefinitions::GamepadButton::LB:
        return m_GamepadLeft && !m_PreviousGamepadLeft;

    case InputDefinitions::GamepadButton::RIGHT_THUMB:
        return m_GamepadRightThumbButton && !m_PreviousGamepadRightThumbButton;

    case InputDefinitions::GamepadButton::LEFT_THUMB:
        return m_GamepadLeftThumbButton && !m_PreviousGamepadLeftThumbButton;

    case InputDefinitions::GamepadButton::DPAD_UP:
        return m_GamepadDpadUp && !m_PreviousGamepadDpadUp;

    case InputDefinitions::GamepadButton::DPAD_RIGHT:
        return m_GamepadDpadRight && !m_PreviousGamepadDpadRight;

    case InputDefinitions::GamepadButton::DPAD_DOWN:
        return m_GamepadDpadDown && !m_PreviousGamepadDpadDown;

    case InputDefinitions::GamepadButton::DPAD_LEFT:
        return m_GamepadDpadLeft && !m_PreviousGamepadDpadLeft;

    default:
        assert(false);
        return 0;
    }
}

bool CInputManager::GamepadButtonBecomesReleased(InputDefinitions::GamepadButton button) const
{

    switch (button)
    {

    case InputDefinitions::GamepadButton::A:
        return !m_GamepadA && m_PreviousGamepadA;

    case InputDefinitions::GamepadButton::B:
        return !m_GamepadB && m_PreviousGamepadB;

    case InputDefinitions::GamepadButton::X:
        return !m_GamepadX && m_PreviousGamepadX;

    case InputDefinitions::GamepadButton::Y:
        return !m_GamepadY && m_PreviousGamepadY;

    case InputDefinitions::GamepadButton::RB:
        return !m_GamepadRight && m_PreviousGamepadRight;

    case InputDefinitions::GamepadButton::LB:
        return !m_GamepadLeft && m_PreviousGamepadLeft;

    case InputDefinitions::GamepadButton::RIGHT_THUMB:
        return !m_GamepadRightThumbButton && m_PreviousGamepadRightThumbButton;

    case InputDefinitions::GamepadButton::LEFT_THUMB:
        return !m_GamepadLeftThumbButton && m_PreviousGamepadLeftThumbButton;

    case InputDefinitions::GamepadButton::DPAD_UP:
        return !m_GamepadDpadUp && m_PreviousGamepadDpadUp;

    case InputDefinitions::GamepadButton::DPAD_RIGHT:
        return !m_GamepadDpadRight && m_PreviousGamepadDpadRight;

    case InputDefinitions::GamepadButton::DPAD_DOWN:
        return !m_GamepadDpadDown && m_PreviousGamepadDpadDown;

    case InputDefinitions::GamepadButton::DPAD_LEFT:
        return !m_GamepadDpadLeft && m_PreviousGamepadDpadLeft;

    default:
        assert(false);
        return 0;
    }
}

float CInputManager::GetGamepadAxis(InputDefinitions::GamepadAxis axis) const
{

    switch (axis)
    {
    case InputDefinitions::LEFT_THUMB_X:
        return m_GamepadStickLeftX;

    case InputDefinitions::LEFT_THUMB_Y:
        return -m_GamepadStickLeftY;

    case InputDefinitions::RIGHT_THUMB_X:
        return m_GamepadStickRightX;

    case InputDefinitions::RIGHT_THUMB_Y:
        return -m_GamepadStickRightY;

    case InputDefinitions::LEFT_TRIGGER:
        return m_GamepadTriggerLeft;

    case InputDefinitions::RIGHT_TRIGGER:
        return m_GamepadTriggerRight;

    default:
        assert(false);
        return 0;
    }
}