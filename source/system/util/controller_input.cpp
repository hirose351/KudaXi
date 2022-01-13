#include		"controller_input.h"

void ControllerInput::Input()
{
	DWORD dwResult;
	for (DWORD i = 0; i < 1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected 

			//右スティックのデッドゾーンの設定
			if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbRX = 0;
				state.Gamepad.sThumbRY = 0;
				mRXFlg = false;
				mRYFlg = false;
				mRXCnt = 0;
				mRYCnt = 0;
			}
			mRStick.x = state.Gamepad.sThumbRX;
			mRStick.y = state.Gamepad.sThumbRY;

			//左スティックのデッドゾーンの設定
			if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbLX = 0;
				state.Gamepad.sThumbLY = 0;
				mLXFlg = false;
				mLYFlg = false;
				mLXCnt = 0;
				mLYCnt = 0;
			}
			mLStick.x = state.Gamepad.sThumbLX;
			mLStick.y = state.Gamepad.sThumbLY;

			//右トリガーのデッドゾーン設定
			if (state.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				state.Gamepad.bRightTrigger = 0;
				mRTFlg = false;
			}
			mRTrigger = state.Gamepad.bRightTrigger;

			//左トリガーのデッドゾーン設定
			if (state.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				state.Gamepad.bLeftTrigger = 0;
				mLTFlg = false;
			}
			mLTrigger = state.Gamepad.bLeftTrigger;

			mState = state;
		}
		else
		{
			// Controller is not connected
		}
	}
}

XINPUT_STATE ControllerInput::GetControllerState()
{
	return mState;
}

DirectX::XMFLOAT2 ControllerInput::GetRStickValue()
{
	return mRStick;
}

bool ControllerInput::GetRStickXDown(int i, bool j)
{
	if (j&&mRXFlg) return false;

	if (i == 0)
	{
		if (mRStick.x != 0)
		{
			mRXFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mRStick.x > i)
		{
			mRXFlg = true;
			return true;
		}
	}
	else if (i < 0)
	{
		if (mRStick.x < i)
		{
			mRXFlg = true;
			return true;
		}
	}
	return false;
}

bool ControllerInput::GetRStickYDown(int i, bool j)
{
	if (j&&mRYFlg) return false;

	if (i == 0)
	{
		if (mRStick.y != 0)
		{
			mRYFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mRStick.y > i)
		{
			mRYFlg = true;
			return true;
		}
	}
	else if (i < 0)
	{
		if (mRStick.y < i)
		{
			mRYFlg = true;
			return true;
		}
	}
	return false;
}

bool ControllerInput::GetRStickXButton(int i, int j, int k)
{
	if (i > 0)
	{
		if (mState.Gamepad.sThumbRX > i)
		{
			mRXCnt++;
			if (mRXCnt == 1 || (mRXCnt > j&&mRXCnt%k == 0))
			{
				return true;
			}
		}
	}
	else if (i < 0)
	{
		if (mState.Gamepad.sThumbRX < i)
		{
			mRXCnt++;
			if (mRXCnt == 1 || (mRXCnt > j&&mRXCnt%k == 0))
			{
				return true;
			}
		}
	}
	return false;
}

bool ControllerInput::GetRStickYButton(int i, int j, int k)
{
	if (i > 0)
	{
		if (mState.Gamepad.sThumbRY > i)
		{
			mRYCnt++;
			if (mRYCnt == 1 || (mRYCnt > j&&mRYCnt%k == 0))
			{
				return true;
			}
		}
	}
	else if (i < 0)
	{
		if (mState.Gamepad.sThumbRY < i)
		{
			mRYCnt++;
			if (mRYCnt == 1 || (mRYCnt > j&&mRYCnt%k == 0))
			{
				return true;
			}
		}
	}
	return false;
}

DirectX::XMFLOAT2 ControllerInput::GetLStickValue()
{
	return mLStick;
}

bool ControllerInput::GetLStickXDown(int i, bool j)
{
	if (j&&mLXFlg) return false;

	if (i == 0)
	{
		if (mLStick.x != 0)
		{
			mLXFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mLStick.x > i)
		{
			mLXFlg = true;
			return true;
		}
	}
	else if (i < 0)
	{
		if (mLStick.x < i)
		{
			mLXFlg = true;
			return true;
		}
	}
	return false;
}

bool ControllerInput::GetLStickYDown(int i, bool j)
{
	if (j&&mLYFlg) return false;

	if (i == 0)
	{
		if (mLStick.y != 0)
		{
			mLYFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mLStick.y > i)
		{
			mLYFlg = true;
			return true;
		}
	}
	else if (i < 0)
	{
		if (mLStick.y < i)
		{
			mLYFlg = true;
			return true;
		}
	}
	return false;
}

bool ControllerInput::GetLStickXButton(int i, int j, int k)
{
	if (i > 0)
	{
		if (mState.Gamepad.sThumbLX > i)
		{
			mLXCnt++;
			if (mLXCnt == 1 || (mLXCnt > j&&mLXCnt%k == 0))
			{
				return true;
			}
		}
	}
	else if (i < 0)
	{
		if (mState.Gamepad.sThumbLX < i)
		{
			mLXCnt++;
			if (mLXCnt == 1 || (mLXCnt > j&&mLXCnt%k == 0))
			{
				return true;
			}
		}
	}
	return false;
}

bool ControllerInput::GetLStickYButton(int i, int j, int k)
{
	if (i > 0)
	{
		if (mState.Gamepad.sThumbLY > i)
		{
			mLYCnt++;
			if (mLYCnt == 1 || (mLYCnt > j&&mLYCnt%k == 0))
			{
				return true;
			}
		}
	}
	else if (i < 0)
	{
		if (mState.Gamepad.sThumbLY < i)
		{
			mLYCnt++;
			if (mLYCnt == 1 || (mLYCnt > j&&mLYCnt%k == 0))
			{
				return true;
			}
		}
	}
	return false;
}

int ControllerInput::GetRTriggerValue()
{
	return mRTrigger;
}

bool ControllerInput::GetRTriggerDown(int i, bool j)
{
	if (j&&mRTFlg) return false;

	if (i == 0)
	{
		if (mRTrigger != 0)
		{
			mRTFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mRTrigger > i)
		{
			mRTFlg = true;
			return true;
		}
	}
	return false;
}

int ControllerInput::GetLTriggerValue()
{
	return mLTrigger;
}

bool ControllerInput::GetLTriggerDown(int i, bool j)
{
	if (j&&mLTFlg) return false;

	if (i == 0)
	{
		if (mLTrigger != 0)
		{
			mLTFlg = true;
			return true;
		}
	}
	else if (i > 0)
	{
		if (mLTrigger > i)
		{
			mLTFlg = true;
			return true;
		}
	}
	return false;
}

bool ControllerInput::GetButtonState(int i)
{
	if (mState.Gamepad.wButtons & i)
	{
		return true;
	}
	return false;
}

bool ControllerInput::GetButtonStateTrigger(int i)
{
	if (mState.Gamepad.wButtons & i)
	{
		if (!mButtonFlg)
		{
			mButtonFlg = true;
			return true;
		}
	}
	else
	{
		mButtonFlg = false;
	}
	return false;
}
