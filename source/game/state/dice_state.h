#pragma once
#include	"state.h"

// �T�C�R���̖ʂ̎��
enum class DICEFRUIT {
	PEACH,
	MELON,
	BLUEBERRY,
	BANANA,
	APPLE,
	ORANGE,
	DICETYPE_MAX,
};

// �T�C�R���̏��
enum class DICESTATUS {
	NORMAL,		// �ʏ�
	UP,			// �オ��
	HALF_UP,	// �オ��
	ROLL,		// ��]
	PUSH,		// ����
	DOWN,		// ����
	HALFDOWN,	// ��������
};

class DiceState : public State
{
};

