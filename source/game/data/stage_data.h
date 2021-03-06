#pragma once
#include		"../../system/util/vector.h"
#include		<vector>
#define		STAGESIZEMAX			(10)


class StageData
{
public:
	StageData() { mStageName = "Test"; }
	~StageData() { Finalize(); }

	void Finalize()
	{
	}

	std::string mStageName;

	int mMapSizeWidth = 8;		// マップ横サイズ
	int mMapSizeHeight = 8;		// マップ縦サイズ

	// 床配置用マップ配列
	int mFloorMap[STAGESIZEMAX][STAGESIZEMAX] = {
		// 0:なし 1:床アリ 
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
	};

	// ブロック生成用マップ配列
	int mMap[10][10] = {
		// 0:なし 1:Dice配置
		// 0:なし 1:Dice配置
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1, 1, 1,-1, 1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	};

	int mDiceCnt;									// Diceの総数 
	std::vector<DirectX::XMFLOAT4X4> mDiceMtx;		// 各サイコロの行列

	int mTargetDiceType = 3;						// 揃えるサイコロの面
	int mStep = 5;									// サイコロの移動可能数

	INT2 mPlayerPos;								// プレイヤーの初期位置

	float mMapChipSize = 15.0f;

	int GetFloorMap(int _cnt)
	{
		return mFloorMap[_cnt%mMapSizeWidth][_cnt / mMapSizeWidth];
	}
};

