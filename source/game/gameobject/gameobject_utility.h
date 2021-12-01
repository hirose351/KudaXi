#pragma once
#define	DICESCALE			(16.2f)

// 移動方向
enum class DIRECTION {
	NEUTRAL,			// 回転していない
	UP,				// 上方向に回転
	DOWN,			// 下方向に回転
	LEFT,			// 左方向に回転
	RIGHT			// 右方向に回転
};
//オブジェクトタイプ
enum class ObjectType
{
	Player = 0b00,
	Dice = 0b01,
	Obstracle = 0b11,
};

enum class ObjectState
{
	EActive,
	EPaused,
	EDead
};

enum class ColliderTag
{
	Floor,
	DiceTop,
	Dice,
};
