#pragma once
#include "state.h"

class State;

namespace PlayerState {
	class Push : public State
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