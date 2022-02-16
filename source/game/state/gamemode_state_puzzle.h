#pragma once
#include	"gamemode_state_base.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Puzzle : public GameModeBase
	{
	private:
		int mStep;							// ステップ数
		bool mIsClear = false;				// クリアしたか
		bool mIsAgain = false;				// ステップ使い果たしたか
		bool mIsCameraMove = false;			// 戻るボタン押されたか
		bool mIsStart = false;				// 始まったか

		Dix::wp<GameObject> mUiStageNum;	// ステージ番号のUI
		Dix::wp<GameObject> mUiStepNum;		// ステップ数のUI
		Dix::wp<GameObject> mUiClearOver;	// クリアオーバー時のUI
		Dix::wp<GameObject> mCameraEye;
		Dix::wp<GameObject> mCameraLookat;

	public:
		Puzzle();
		~Puzzle();

		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}