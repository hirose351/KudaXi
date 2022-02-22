#pragma once
#include	"gamemode_state_base.h"
#include	"../component/quad2d_component.h"

namespace GameModeState {
	class Puzzle : public GameModeBase
	{
	private:
		int mStep;							// �X�e�b�v��
		bool mIsClear = false;				// �N���A������
		bool mIsAgain = false;				// �X�e�b�v�g���ʂ�������
		bool mIsCameraMove = false;			// �߂�{�^�������ꂽ��
		bool mIsStart = false;				// �n�܂�����

		Dix::wp<GameObject> mpUiStageNum;	// �X�e�[�W�ԍ���UI
		Dix::wp<GameObject> mpUiStepNum;	// �X�e�b�v����UI
		Dix::wp<GameObject> mpUiClearOver;	// �N���A�I�[�o�[����UI
		Dix::wp<GameObject> mpCameraEye;
		Dix::wp<GameObject> mpCameraLookat;

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