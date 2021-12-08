#pragma once
#include	"state.h"

// サイコロの面の種類
enum class DICEFRUIT {
	PEACH,
	MELON,
	BLUEBERRY,
	BANANA,
	APPLE,
	ORANGE,
	DICETYPE_MAX,
};

// サイコロの状態
enum class DICESTATUS {
	NORMAL,		// 通常
	UP,			// 上がる
	HALF_UP,	// 上がる
	ROLL,		// 回転
	PUSH,		// 押す
	DOWN,		// 沈む
	HALFDOWN,	// 半分沈む
};

class DiceState : public State
{
};

