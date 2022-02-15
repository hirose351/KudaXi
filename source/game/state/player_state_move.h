#pragma once
#include "player_state_base.h"

namespace PlayerState {
	class Move : public PlayerStateBase
	{
	private:
		Float3	mDestrot;				// �ڕW�̎p��
		INT2	mStageSize;				// �X�e�[�W�̑傫��
		unsigned int mDiceDownFrame;	// Dice����~���ׂ̒������̃t���[����

		bool CheckPush();
		bool CheckRoll();
		void SetMapPos();

		// �T�C�R������o�Ȃ��悤�ړ�����
		void MoveLimitDice(INT3 _dicePos);

	public:
		// ��������
		void Init()override;
		// ���s
		void Exec()override;
		// State�ύX�O����
		void BeforeChange()override;
		// State�ύX�㏈��
		void AfterChange()override;
	};
}
