#pragma once
#include	"gamemode_state_base.h"

namespace GameModeState {
	class Endless : public GameModeBase
	{
	private:
		Dix::wp<GameObject> mpPauseBt;
		Dix::wp<GameObject> mpOverImage;
		Dix::wp<GameObject> mpScoreNum;
		bool mIsOver = false;
		/// Todo:スコアの処理はここで管理したい

	public:
		Endless();
		~Endless();

		// 操作する変数登録
		void Start(Component::GameModeController* _c)override;
		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}