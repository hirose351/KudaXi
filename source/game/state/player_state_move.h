#pragma once
#include "player_state_base.h"

namespace PlayerState {
	class Move : public PlayerStateBase
	{
	private:
		Float3	mDestrot;			// �ڕW�p��
		INT3	mStageSize;

		bool CheckPush();
		bool CheckRoll();
		void SetMapPos();
	public:
		// ��������
		void Init()override;
		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}
