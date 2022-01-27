#pragma once
#include	"gamemode_state_base.h"

namespace GameModeState {
	class Endless : public GameModeBase
	{
	private:
		bool mIsOver = false;		// �I�[�o�[������

		/// Todo:�X�R�A�̏����͂����ŊǗ�������
	public:
		Endless();
		~Endless();

		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}