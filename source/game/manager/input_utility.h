#pragma once

enum class InputMode
{
	eUi,
	eGame,
};

enum class UiAction
{
	eNavigate,		// 操作(方向)
	eClick,			// 決定(bool)
	eCancel,		// 戻る(bool)
	ePause,			// ポーズ(bool)
	MaxUiAction
};

enum class GameAction
{
	Move,			// 移動(方向)
	MaxGameAction
};

enum class InputDirection
{
	eNeutral,			// 無し
	eUp,					// 上
	eDown,				// 下
	eLeft,				// 左
	eRight				// 右
};