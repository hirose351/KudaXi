#pragma once
#include "state.h"

namespace PlayerState {
	class Move : public State
	{
	private:
		Float3	mDestrot;			// �ڕW�p��
		INT3	stageSize;

		void CheckPush();
		void CheckRoll();
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
