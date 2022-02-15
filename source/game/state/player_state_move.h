#pragma once
#include "player_state_base.h"

namespace PlayerState {
	class Move : public PlayerStateBase
	{
	private:
		Float3	mDestrot;				// 目標の姿勢
		INT2	mStageSize;				// ステージの大きさ
		unsigned int mDiceDownFrame;	// Diceから降りる為の長押しのフレーム数

		bool CheckPush();
		bool CheckRoll();
		void SetMapPos();

		// サイコロから出ないよう移動制限
		void MoveLimitDice(INT3 _dicePos);

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
