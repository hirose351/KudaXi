#pragma once
#include "player_state_base.h"

class PlayerStateBase;

namespace PlayerState {
	class Push : public PlayerStateBase
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