#pragma once
#include	"gamemode_state_base.h"
#include	"../gameobject/ui_button.h"

namespace GameModeState {
	class Select : public GameModeBase
	{
	private:
		Dix::wp<myUI::ButtonGroup> mButton;

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