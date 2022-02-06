#pragma once

enum class InputMode
{
	eUi,
	eGame,
};

enum class UiAction
{
	eNavigate,		// ����(����)
	eClick,			// ����(bool)
	eCancel,		// �߂�(bool)
	eCancel2,		// �߂�(bool)
	ePause,			// �|�[�Y(bool)
	eSwitch,		// �؂�ւ�(bool)
	MaxUiAction
};

enum class GameAction
{
	Move,			// �ړ�(����)
	MaxGameAction
};

enum class InputDirection
{
	eNeutral,			// ����
	eUp,					// ��
	eDown,				// ��
	eLeft,				// ��
	eRight				// �E
};