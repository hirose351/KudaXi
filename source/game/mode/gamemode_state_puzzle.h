#pragma once
#include	"gamemode_state_base.h"

namespace GameMode {
	class Puzzle : public GameModeBase
	{
	private:
		int mStep;	// �X�e�b�v��

		bool mIsClear = false;		// �N���A������
		bool mIsAgain = false;		// �X�e�b�v�g���ʂ�������

	public:
		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}