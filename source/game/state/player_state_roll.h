#pragma once
#include "state.h"

namespace PlayerState {
	class Roll : public State
	{
	private:
	public:
		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}
