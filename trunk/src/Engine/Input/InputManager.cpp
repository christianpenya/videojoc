#include "InputManager.h"

void engine::input::CInputManager::PreUpdate(bool isWindowActive) {

	// TODO Falta setejar algunes variables

	m_PreviousKeyboardState = m_KeyboardState;

	m_PreviousButtonLeft = m_ButtonLeft;
	m_PreviousButtonMiddle = m_ButtonMiddle;
	m_PreviousButtonRight = m_ButtonRight;

	m_MouseMovementX = m_MouseMovementY = m_MouseMovementZ = 0;

	if (!isWindowActive) {
		m_KeyboardState = {};
		m_ButtonLeft = m_ButtonMiddle = m_ButtonRight = false;
	}
}

void engine::input::CInputManager::PostUpdate() {

}

bool engine::input::CInputManager::HandleKeyboard(const MSG& msg) {
	
	bool handled = false;

	unsigned char VKCode = (unsigned char)msg.wParam;

	int repeat = msg.lParam & 0xffff;
	bool AltKeyWasDown = ((msg.lParam & (1 << 29)) != 0);
	bool WasDown = ((msg.lParam & (1 << 30)) != 0);

	bool IsDown = ((msg.lParam & (1 << 31)) == 0);

	if (WasDown != IsDown) 	{
		m_KeyboardState.SetKey(VKCode, IsDown);
		handled = true;
	}

	return handled;
}

bool engine::input::CInputManager::HandleMouse(const MSG& msg) {
	
	switch (msg.message) {
	
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
	
	case WM_INPUT: {
		UINT dwSize = 60;
		static BYTE lpb[60];

		UINT read = GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE && read <= dwSize) {
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

void engine::input::CInputManager::KeyboardData::SetKey(unsigned char key, bool state) {
	
	raw[key] = state;

	if (key >= VK_NUMPAD0 && key <= VK_NUMPAD9) {
		numpad[key - VK_NUMPAD0] = state;
	}
	else if (key >= VK_F1 && key <= VK_F24) {
		fkey[key - VK_F1] = state;
	}
	else {
		switch (key) {
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

void engine::input::CInputManager::SetupHighprecisionMouse(HWND hWnd) {
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;
	m_HighPrecisionMouseRegistered = (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) != 0);

}

int engine::input::CInputManager::GetMouseAxis(InputDefinitions::MouseAxis axis) const {
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
