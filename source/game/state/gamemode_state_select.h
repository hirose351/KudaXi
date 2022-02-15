#pragma once
#include	"gamemode_state_base.h"
#include	"../gameobject/ui_button.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Select : public GameModeBase
	{
	private:
		Dix::wp<myUI::ButtonGroup> mButton;				// �X�e�[�W�I���{�^��
		std::vector<Component::Quad2d*> mStageNumQuad;	// �X�e�[�W�ԍ��I�u�W�F�N�g
		int mStageNum = 1;

		// �X�e�[�W�ݒ�
		void SetStage();

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