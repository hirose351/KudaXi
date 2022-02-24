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
		/// Todo:�X�R�A�̏����͂����ŊǗ�������

	public:
		Endless();
		~Endless();

		// ���삷��ϐ��o�^
		void Start(Component::GameModeController* _c)override;
		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}