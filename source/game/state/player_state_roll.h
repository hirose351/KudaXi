#pragma once
#include "state.h"

namespace PlayerState {
	class Roll : public State
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
