#pragma once

// サイコロの面の種類
enum class DiceFruit {
	ePeach,
	eMelon,
	eBlueberry,
	eBanana,
	eApple,
	eOrange,
	eDiceFruitMax,
};

// サイコロの状態
enum class DiceStatus {
	eNormal,		// 通常
	eUp,			// 上がる
	eHalfUp,	// 上がる
	eRoll,		// 回転
	ePush,		// 押す
	eDown,		// 沈む
	eHalfDown,	// 半分沈む
};