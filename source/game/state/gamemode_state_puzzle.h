#pragma once
#include	"gamemode_state_base.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Puzzle : public GameModeBase
	{
	private:
		int mStep;	// �X�e�b�v��

		bool mIsClear = false;		// �N���A������
		bool mIsAgain = false;		// �X�e�b�v�g���ʂ�������
		//bool mIsSelect = false;		// �I�����ꂽ��

		Component::Quad2d* mUiStage;	// �X�e�[�W��UI
		Component::Quad2d* mUiStageNum;	// �X�e�[�W�ԍ���UI
		Component::Quad2d* mUiStep;		// �X�e�b�v��UI
		Component::Quad2d* mUiStepNum;	// �X�e�b�v����UI
		Component::Quad2d* mUiClearOver;// �N���A�I�[�o�[����UI

	public:
		Puzzle();
		~Puzzle();

		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}