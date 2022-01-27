#pragma once
#include	"gamemode_state_base.h"
#include	"../gameobject/ui_button.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Select : public GameModeBase
	{
	private:
		Dix::wp<myUI::ButtonGroup> mButton;
		Dix::wp<Component::Quad2d> mStageNum;// �X�e�[�W�ԍ��I�u�W�F�N�g

	public:
		Select();
		~Select();

		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}