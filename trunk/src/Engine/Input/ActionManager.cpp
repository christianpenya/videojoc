#include "ActionManager.h"
#include <assert.h>

CActionManager::CActionManager(const CInputManager &inputManager)
	:m_InputManager(inputManager){
}

ActionTrigger::TriggerType ActionTrigger::GetTriggerTypeFromString(const char* str, TriggerType defaultValue)
{
	if (str == nullptr)
		return defaultValue;
	else if (strcmp(str, "KEYBOARD") == 0)
		return KEYBOARD;
	else if (strcmp(str, "MOUSE") == 0)
		return MOUSE;
	else if (strcmp(str, "MOUSE_BUTTON") == 0)
		return MOUSE_BUTTON;
	else if (strcmp(str, "GAMEPAD") == 0)
		return GAMEPAD;
	else if (strcmp(str, "GAMEPAD_BUTTON") == 0)
		return GAMEPAD_BUTTON;
	else
		return (TriggerType)-1;
}

ActionTrigger::ButtonActionType ActionTrigger::GetButtonActionTypeFromString(const char* str, ButtonActionType defaultValue)
{
	if (str == nullptr)
		return defaultValue;
	else if (strcmp(str, "IsPressed") == 0)
		return IsPressed;
	else if (strcmp(str, "IsReleased") == 0)
		return IsReleased;
	else if (strcmp(str, "BecomesPressed") == 0)
		return BecomesPressed;
	else if (strcmp(str, "BecomesReleased") == 0)
		return BecomesReleased;
	else
		return (ButtonActionType)-1;
}

void CActionManager::Update() {

	for (auto &actionIt : m_ResourcesMap) {

		InputAction *action = actionIt.second;
		action->active = false;
		action->value = 0;

		for (ActionTrigger& trigger : action->triggers) {
			switch (trigger.type)
			{
			case ActionTrigger::KEYBOARD:
				switch (trigger.keyboard.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsKeyPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsKeyPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesPressed:
					if (m_InputManager.KeyBecomesPressed(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				case ActionTrigger::BecomesReleased:
					if (m_InputManager.KeyBecomesReleased(trigger.keyboard.keyCode))
					{
						action->active = true;
						action->value = trigger.keyboard.value;
					}
					break;
				default:
					assert(false);
					break;
				}
				break;
			case ActionTrigger::MOUSE:
			{
				float value = m_InputManager.GetMouseAxis(trigger.mouse.axis);
				bool active = trigger.mouse.geThreshold ? (value >= trigger.mouse.threshold) : (value <= trigger.mouse.threshold);
				if (fabs(value) >= fabs(action->value))
				{
					if (active || !action->active)
					{
						action->value = value;
						action->active = active;
					}
				}
				break;
			}
			case ActionTrigger::MOUSE_BUTTON:
				switch (trigger.mouseButton.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsMouseButtonPressed(trigger.mouseButton.button)) {
						action->active = true;
						action->value = trigger.mouseButton.value;
					}
					break;
				
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsMouseButtonPressed(trigger.mouseButton.button)) {
						action->active = true;
						action->value = trigger.mouseButton.value;
					}
					break;

				case ActionTrigger::BecomesPressed:
					if (m_InputManager.MouseButtonBecomesPressed(trigger.mouseButton.button)) {
						action->active = true;
						action->value = trigger.mouseButton.value;
					}
					break;

				case ActionTrigger::BecomesReleased:
					if (m_InputManager.MouseButtonBecomesReleased(trigger.mouseButton.button)) {
						action->active = true;
						action->value = trigger.mouseButton.value;
					}
					break;
				default:
					assert(false);
					break;
				}
				break;
			case ActionTrigger::GAMEPAD:
			{
				float value = m_InputManager.GetGamepadAxis(trigger.gamepad.axis);
				bool active = trigger.gamepad.geThreshold ? (value >= trigger.gamepad.threshold) : (value <= trigger.gamepad.threshold);
				if (fabs(value) >= fabs(action->value))
				{
					if (active || !action->active)
					{
						action->value = value;
						action->active = active;
					}
				}
				break;
			}
			case ActionTrigger::GAMEPAD_BUTTON:
				switch (trigger.mouseButton.actionType)
				{
				case ActionTrigger::IsPressed:
					if (m_InputManager.IsGamepadButtonPressed(trigger.gamepadButton.button)) {
						action->active = true;
						action->value = trigger.gamepadButton.value;
					}
					break;
				case ActionTrigger::IsReleased:
					if (!m_InputManager.IsGamepadButtonPressed(trigger.gamepadButton.button)) {
						action->active = true;
						action->value = trigger.mouseButton.value;
					}
					break;

				case ActionTrigger::BecomesPressed:
					if (m_InputManager.GamepadButtonBecomesPressed(trigger.gamepadButton.button)) {
						action->active = true;
						action->value = trigger.gamepadButton.value;
					}
					break;

				case ActionTrigger::BecomesReleased:
					if (m_InputManager.GamepadButtonBecomesReleased(trigger.gamepadButton.button)) {
						action->active = true;
						action->value = trigger.gamepadButton.value;
					}
					break;

				default:
					assert(false);
					break;
				}
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

inline unsigned char GetKeyCode(const std::string& str)
{
	if (str.length() == 1 && ((str[0] >= 'A' && str[0] <= 'Z')
		|| (str[0] >= '0' && str[0] <= '9'))
		|| str[0] >= 0x20 && str[0] <= 0x20F) // Tecles especials
	{
		return str[0];
	}
	else if (str == "SPACE" || str == " ")
	{
		return VK_SPACE;
	}
	else
	{
		assert(false);
		return -1;
	}
}

bool CActionManager::LoadActions(const std::string &path)
{
	bool ok = false;

	tinyxml2::XMLDocument document;
	tinyxml2::XMLError error = document.LoadFile(path.c_str());
	if (base::xml::SucceedLoad(error))
	{
		tinyxml2::XMLElement *actions = document.FirstChildElement("actions");
		if (actions)
		{
			for (tinyxml2::XMLElement *action = actions->FirstChildElement(); action != nullptr; action = action->NextSiblingElement())
			{
				if (strcmp(action->Name(), "action") == 0)
				{
					InputAction inputAction = {};
					const char *actionName = action->Attribute("name");
					assert(actionName != nullptr);

					for (tinyxml2::XMLElement *trigger = action->FirstChildElement(); trigger != nullptr; trigger = trigger->NextSiblingElement())
					{
						if (strcmp(trigger->Name(), "trigger") == 0)
						{
							ActionTrigger actionTrigger = {};

							actionTrigger.type = ActionTrigger::GetTriggerTypeFromString(trigger->Attribute("type"));
							switch (actionTrigger.type)
							{
							case ActionTrigger::KEYBOARD:
								actionTrigger.keyboard.keyCode = GetKeyCode(trigger->Attribute("key_code"));
								actionTrigger.keyboard.actionType = ActionTrigger::GetButtonActionTypeFromString(trigger->Attribute("button_type"), ActionTrigger::IsPressed);
								actionTrigger.keyboard.value = trigger->FloatAttribute("value");
								break;
							case ActionTrigger::MOUSE:
								actionTrigger.mouse.axis = InputDefinitions::GetMouseAxisFromString(trigger->Attribute("axis"), InputDefinitions::DX);
								actionTrigger.mouse.threshold = trigger->FloatAttribute("threshold");
								actionTrigger.mouse.geThreshold = trigger->BoolAttribute("ge_threshold");
								break;
							case ActionTrigger::MOUSE_BUTTON:
								actionTrigger.mouseButton.button = InputDefinitions::GetMouseButtonFromString(trigger->Attribute("button"));
								actionTrigger.mouseButton.actionType = ActionTrigger::GetButtonActionTypeFromString(trigger->Attribute("button_type"), ActionTrigger::IsPressed);
								actionTrigger.mouseButton.value = trigger->FloatAttribute("value");
								break;
							case ActionTrigger::GAMEPAD:
								actionTrigger.gamepad.axis = InputDefinitions::GetGamepadAxisFromString(trigger->Attribute("axis"), InputDefinitions::RIGHT_THUMB_X);
								actionTrigger.gamepad.threshold = trigger->FloatAttribute("threshold");
								actionTrigger.gamepad.geThreshold = trigger->BoolAttribute("ge_threshold");
								break;
							case ActionTrigger::GAMEPAD_BUTTON:
								actionTrigger.gamepadButton.button = InputDefinitions::GetGamepadButtonFromString(trigger->Attribute("button"));
								actionTrigger.gamepadButton.actionType = ActionTrigger::GetButtonActionTypeFromString(trigger->Attribute("button_type"), ActionTrigger::IsPressed);
								actionTrigger.gamepadButton.value = trigger->FloatAttribute("value");
								break;
							default:
								assert(false);
								break;
							}

							inputAction.triggers.push_back(actionTrigger);
						}
						else
						{
							assert(false); // TODO better log error
						}
					}

					base::utils::CTemplatedMap<InputAction>::Update(actionName, new InputAction(inputAction));
				}
				else {
					assert(false); // TODO better log error
				}
			}
		}
		ok = true;
	}
	return ok;
}
