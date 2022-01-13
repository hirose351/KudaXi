#pragma once

enum class InputMode
{
	eUi,
	eGame,
};

enum class UiAction
{
	eNavigate,		// ‘€ì(•ûŒü)
	eClick,			// Œˆ’è(bool)
	eCancel,			// –ß‚é(bool)
	MaxUiAction
};

enum class GameAction
{
	Move,			// ˆÚ“®(•ûŒü)
	MaxGameAction
};

enum class InputDirection
{
	eNeutral,			// –³‚µ
	eUp,					// ã
	eDown,				// ‰º
	eLeft,				// ¶
	eRight				// ‰E
};