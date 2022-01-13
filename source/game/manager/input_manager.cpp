#include	"input_manager.h"
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/util/controller_input.h"

bool InputManager::GetState(InputMode _mode, int _action)
{
	switch (_mode)
	{
	case InputMode::eUi:
	{
		if (_action >= static_cast<int>(UiAction::MaxUiAction))
			break;

		UiAction uiAction = static_cast<UiAction>(_action);

		switch (uiAction)
		{
		case UiAction::eClick:
		{
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN) || ControllerInput::GetInstance().GetButtonState(A))
				return true;
		}
		break;
		case UiAction::eCancel:
		{

		}
		break;
		//default:
		//	break;
		}

		break;
		//case InputMode::eGame:
		//	if (_action >= static_cast<int>(GameAction::MaxGameAction))
		//		break;

		//	GameAction gameAction = static_cast<GameAction>(_action);

		//	switch (gameAction)
		//	{
		//	default:
		//		break;
		//	}

		//	break;
		//default:
		//	break;
	}
	}
	return false;
}

bool InputManager::GetStateTrigger(InputMode _mode, int _action)
{
	switch (_mode)
	{
	case InputMode::eUi:
	{
		if (_action >= static_cast<int>(UiAction::MaxUiAction))
			break;

		UiAction uiAction = static_cast<UiAction>(_action);

		switch (uiAction)
		{
		case UiAction::eClick:
		{
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN) || ControllerInput::GetInstance().GetButtonStateTrigger(A))
				return true;
		}
		break;
		case UiAction::eCancel:
		{

		}
		break;
		//default:
		//	break;
		}

		break;
		//case InputMode::eGame:
		//	if (_action >= static_cast<int>(GameAction::MaxGameAction))
		//		break;

		//	GameAction gameAction = static_cast<GameAction>(_action);

		//	switch (gameAction)
		//	{
		//	default:
		//		break;
		//	}

		//	break;
		//default:
		//	break;
	}
	}
	return false;
}

InputDirection InputManager::GetDirection(InputMode _mode, int _action)
{
	switch (_mode)
	{
	case InputMode::eUi:
	{
		if (_action >= static_cast<int>(UiAction::MaxUiAction))
			break;

		UiAction uiAction = static_cast<UiAction>(_action);

		switch (uiAction)
		{
		case UiAction::eNavigate:
		{
			// 左
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
			{
				return InputDirection::eLeft;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_LEFT))
			{
				return InputDirection::eLeft;
			}
			// 右
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
			{
				return InputDirection::eRight;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_RIGHT))
			{
				return InputDirection::eRight;
			}
			// 上
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
			{
				return InputDirection::eUp;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_UP))
			{
				return InputDirection::eUp;
			}
			// 下
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{
				return InputDirection::eDown;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_DOWN))
			{
				return InputDirection::eDown;
			}
		}
		break;
		//default:
		//	break;
		}
	}
	break;
	case InputMode::eGame:
	{
		if (_action >= static_cast<int>(GameAction::MaxGameAction))
			break;

		GameAction gameAction = static_cast<GameAction>(_action);

		switch (gameAction)
		{
		case GameAction::Move:
			// 左
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
			{
				return InputDirection::eLeft;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_LEFT))
			{
				return InputDirection::eLeft;
			}
			// 右
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
			{
				return InputDirection::eRight;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_RIGHT))
			{
				return InputDirection::eRight;
			}
			// 上
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
			{
				return InputDirection::eUp;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_UP))
			{
				return InputDirection::eUp;
			}
			// 下
			if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
			{
				return InputDirection::eDown;
			}
			if (ControllerInput::GetInstance().GetButtonState(DPAD_DOWN))
			{
				return InputDirection::eDown;
			}
		}
		break;
		//default:
		//	break;
	}

	break;
	//default:
	//	break;
	}

	return InputDirection::eNeutral;
}

InputDirection InputManager::GetDirectionTrigger(InputMode _mode, int _action)
{
	switch (_mode)
	{
	case InputMode::eUi:
	{
		if (_action >= static_cast<int>(UiAction::MaxUiAction))
			break;

		UiAction uiAction = static_cast<UiAction>(_action);

		switch (uiAction)
		{
		case UiAction::eNavigate:
			// 左
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
			{
				return InputDirection::eLeft;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_LEFT))
			{
				return InputDirection::eLeft;
			}
			// 右
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_D))
			{
				return InputDirection::eRight;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_RIGHT))
			{
				return InputDirection::eRight;
			}
			// 上
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W))
			{
				return InputDirection::eUp;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_UP))
			{
				return InputDirection::eUp;
			}
			// 下
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_S))
			{
				return InputDirection::eDown;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_DOWN))
			{
				return InputDirection::eDown;
			}
		}
		break;
		//default:
		//	break;
	}
	break;
	case InputMode::eGame:
	{
		if (_action >= static_cast<int>(GameAction::MaxGameAction))
			break;

		GameAction gameAction = static_cast<GameAction>(_action);

		switch (gameAction)
		{
		case GameAction::Move:
			// 左
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
			{
				return InputDirection::eLeft;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_LEFT))
			{
				return InputDirection::eLeft;
			}
			// 右
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_D))
			{
				return InputDirection::eRight;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_RIGHT))
			{
				return InputDirection::eRight;
			}
			// 上
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W))
			{
				return InputDirection::eUp;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_UP))
			{
				return InputDirection::eUp;
			}
			// 下
			if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_S))
			{
				return InputDirection::eDown;
			}
			if (ControllerInput::GetInstance().GetButtonStateTrigger(DPAD_DOWN))
			{
				return InputDirection::eDown;
			}
		}
		break;
		//default:
		//	break;
	}
	break;
	//default:
	//	break;
	}

	return InputDirection::eNeutral;
}
