#pragma once
#define	DICE_SCALE			(16.2f)
#define	DICE_SCALE_HALF		(DICE_SCALE/2.0f)

// �ړ�����
enum class Direction
{
	eNeutral,			// ����
	eUp,				// ��
	eDown,				// ��
	eLeft,				// ��
	eRight				// �E
};

//�I�u�W�F�N�g�^�C�v
enum class ObjectType
{
	ePlayer = 0b00,
	eDice = 0b01,
	eObstracle = 0b11,
};

enum class ObjectState
{
	eActive,
	ePaused,
	eDead
};

enum class ObjectTag
{
	Floor,
	DiceTop,
	Dice,
};

static const char *ObjectTagStr[] =
{ "Floor", "DiceTop", "Dice" };
