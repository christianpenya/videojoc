#pragma once
#ifndef _ENGINE_INPUTMANAGER_CPB_1611201620428_H
#define _ENGINE_INPUTMANAGER_CPB_1611201620428_H

#include "InputDefinitions.h"
#include <Windows.h>
#include <assert.h>


namespace engine {
	namespace input {

		class CInputManager {

		public:

			CInputManager(HWND hWnd){ SetupHighprecisionMouse(hWnd);	}
			virtual ~CInputManager(){}

			bool HandleKeyboard(const MSG& msg);
			bool HandleMouse(const MSG& msg);

			void PreUpdate(bool isWindowActive);
			void PostUpdate();

			void SetupHighprecisionMouse(HWND hWnd);

			// ----

			bool IsKeyPressed(unsigned char KeyCode) const { return m_KeyboardState.raw[KeyCode]; }
			bool KeyBecomesPressed(unsigned char KeyCode) const { return m_KeyboardState.raw[KeyCode] && !m_PreviousKeyboardState.raw[KeyCode]; }
			bool KeyBecomesReleased(unsigned char KeyCode) const { return !m_KeyboardState.raw[KeyCode] && m_PreviousKeyboardState.raw[KeyCode]; }

			bool IsEscapePressed(unsigned char KeyCode) const { return m_KeyboardState.escape; }
			bool EscapeBecomesPressed(unsigned char KeyCode) const { return m_KeyboardState.escape && !m_PreviousKeyboardState.escape; }
			bool EscapeBecomesReleased(unsigned char KeyCode) const { return !m_KeyboardState.escape && m_PreviousKeyboardState.escape; }

			bool IsSpacePressed(unsigned char KeyCode) const { return m_KeyboardState.space; }
			bool SpaceBecomesPressed(unsigned char KeyCode) const { return m_KeyboardState.space && !m_PreviousKeyboardState.space; }
			bool SpaceBecomesReleased(unsigned char KeyCode) const { return !m_KeyboardState.space && m_PreviousKeyboardState.space; }

			int GetMouseMovementX() const { return m_MouseMovementX; }
			int GetMouseMovementY() const { return m_MouseMovementY; }
			int GetMouseMovementZ() const { return m_MouseMovementZ; }

			int GetMouseX() const { return m_MouseX; }
			int GetMouseY() const { return m_MouseY; }

			int GetMouseAxis(InputDefinitions::MouseAxis axis) const;

			bool IsLeftMouseButtonPressed() const { return m_ButtonLeft; }
			bool IsMiddleMouseButtonPressed() const { return m_ButtonMiddle; }
			bool IsRightMouseButtonPressed() const { return m_ButtonRight; }

			bool LeftMouseButtonBecomesPressed() const { return m_ButtonLeft && !m_PreviousButtonLeft; }
			bool MiddleMouseButtonBecomesPressed() const { return m_ButtonMiddle && !m_PreviousButtonMiddle; }
			bool RightMouseButtonBecomesPressed() const { return m_ButtonRight && !m_PreviousButtonRight; }

			bool LeftMouseButtonBecomesReleased() const { return !m_ButtonLeft && m_PreviousButtonLeft; }
			bool MiddleMouseButtonBecomesReleased() const { return !m_ButtonMiddle && m_PreviousButtonMiddle; }
			bool RightMouseButtonBecomesReleased() const { return !m_ButtonRight && m_PreviousButtonRight; }

			bool IsMouseButtonPressed(InputDefinitions::MouseButton button) const { return (m_ButtonLeft || m_ButtonMiddle || m_ButtonRight); };
			bool MouseButtonBecomesPressed(InputDefinitions::MouseButton button) const { return (m_ButtonLeft && !m_PreviousButtonLeft || m_ButtonMiddle && !m_PreviousButtonMiddle || m_ButtonRight &&!m_PreviousButtonRight); };
			bool MouseButtonBecomesReleased(InputDefinitions::MouseButton button) const { return (!m_ButtonLeft && m_PreviousButtonLeft || !m_ButtonMiddle && m_PreviousButtonMiddle || !m_ButtonRight &&m_PreviousButtonRight); };

		private:

			bool UpdateKeyboard();
			bool UpdateMouse();

			struct KeyboardData
			{
				bool raw[256] = {};
				bool escape = false, space = false;
				bool numpad[10] = {};
				bool fkey[24] = {};
				bool left = false, right = false, up = false, down = false;

				void SetKey(unsigned char key, bool state);

			} m_PreviousKeyboardState, m_KeyboardState;

			bool m_HighPrecisionMouseRegistered;

			int									m_MouseMovementX = 0;
			int									m_MouseMovementY = 0;
			int									m_MouseMovementZ = 0;
			int									m_MouseX = 0;
			int									m_MouseY = 0;
			bool								m_ButtonRight = false, m_PreviousButtonRight = false;
			bool								m_ButtonLeft = false, m_PreviousButtonLeft = false;
			bool								m_ButtonMiddle = false, m_PreviousButtonMiddle = false;
		};
	}
}

#endif //_ENGINE_INPUTMANAGER_CPB_1611201620428_H

