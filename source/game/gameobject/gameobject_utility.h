#pragma once

// �ړ�����
enum class DIRECTION {
	NEUTRAL,			// ��]���Ă��Ȃ�
	UP,				// ������ɉ�]
	DOWN,			// �������ɉ�]
	LEFT,			// �������ɉ�]
	RIGHT			// �E�����ɉ�]
};
//�I�u�W�F�N�g�^�C�v
enum class ObjectType
{
	Player = 0b00,
	Enemy = 0b01,
	Obstracle = 0b11,
};

enum class ObjectState
{
	EActive,
	EPaused,
	EDead
};
