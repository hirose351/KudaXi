#pragma once
#define	DICE_SCALE			(16.2f)
#define	DICE_SCALE_HALF		(DICE_SCALE/2.0f)

// 移動方向
enum class Direction
{
	eNeutral,			// 無し
	eUp,				// 上
	eDown,				// 下
	eLeft,				// 左
	eRight				// 右
};

static const char *DirectionStr[] =
{ "Neutral", "Up", "Down","Left","Right" };

//オブジェクトタイプ
enum class ObjectType
{
	ePlayer = 0b00,
	eDice = 0b01,
	eStage = 0b01,
	eObstracle = 0b11,
};

enum class ObjectState
{
	eActive,
	ePaused,
	eDead
};

enum class Foot
{
	eNomal,
	eFloor,
	eDice
};

static const char *FootStr[] =
{ "Nomal", "Floor", "Dice" };

enum class ObjectTag
{
	eFloor,
	eDiceTop,
	eDice,
};

static const char *ObjectTagStr[] =
{ "Floor", "DiceTop", "Dice" };
