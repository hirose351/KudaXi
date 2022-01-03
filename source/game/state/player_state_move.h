#pragma once
#include "player_state_base.h"

namespace PlayerState {
	class Move : public PlayerStateBase
	{
	private:
		Float3	mDestrot;			// 目標姿勢
		INT3	mStageSize;

		bool CheckPush();
		bool CheckRoll();
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
