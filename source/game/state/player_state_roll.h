#pragma once
#include "player_state_base.h"

namespace PlayerState {
	class Roll : public PlayerStateBase
	{
	private:
	public:
		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}
