#pragma once
#include	"gamemode_state_base.h"
#include	"../gameobject/ui_button.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Select : public GameModeBase
	{
	private:
		Dix::wp<myUI::ButtonGroup> mButton;				// ステージ選択ボタン
		std::vector<Component::Quad2d*> mStageNumQuad;	// ステージ番号オブジェクト
		int mStageNum = 1;

		// ステージ設定
		void SetStage();

	public:
		Select();
		~Select();

		// 実行
		void Exec()override;
		// State変更前処理
		void BeforeChange()override;
		// State変更後処理
		void AfterChange()override;
	};
}