#pragma once
#include	"gamemode_state_base.h"

namespace GameMode {
	class Puzzle : public GameModeBase
	{
	private:
		int mStep;	// ステップ数

		bool mIsClear = false;		// クリアしたか
		bool mIsAgain = false;		// ステップ使い果たしたか

	public:
		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}