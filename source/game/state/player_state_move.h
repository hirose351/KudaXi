#pragma once
#include "state.h"

namespace PlayerState {
	class Move : public State
	{
	private:
		Float3	mDestrot;			// 目標姿勢
		INT3	stageSize;

		void CheckPush();
		void CheckRoll();
		void SetMapPos();
	public:
		// 初期処理
		void Init()override;
		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}
