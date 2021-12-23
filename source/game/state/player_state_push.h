#pragma once
#include "player_state_base.h"

class PlayerStateBase;

namespace PlayerState {
	class Push : public PlayerStateBase
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