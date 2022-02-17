#pragma once
#include	"gamemode_state_base.h"

namespace GameModeState {
	class Endless : public GameModeBase
	{
	private:
		Dix::wp<GameObject> mPauseBt;
		Dix::wp<GameObject> mOverImage;
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