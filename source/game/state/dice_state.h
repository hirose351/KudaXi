#pragma once
#include	"state.h"

// �T�C�R���̖ʂ̎��
enum class DiceFruit {
	ePeach,
	eMelon,
	eBlueberry,
	eBanana,
	eApple,
	eOrange,
	eDiceFruitMax,
};

// �T�C�R���̏��
enum class DiceStatus {
	eNormal,		// �ʏ�
	eUp,			// �オ��
	eHalfUp,	// �オ��
	eRoll,		// ��]
	ePush,		// ����
	eDown,		// ����
	eHalfDown,	// ��������
};

class DiceState : public State
{
};

