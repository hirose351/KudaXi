#pragma once
#include		"../../system/util/vector.h"
#include		"../../system/util/dixsmartptr.h"
#include		<vector>
#define		STAGESIZEMAX			(10)


class StageData
{
public:
	StageData() {
		mStageName = "Test";
	}

	~StageData() { Finalize(); }

	// 後始末
	void Finalize()
	{
		//if (!mDiceMtx.empty())
		//{
		//	mDiceMtx.clear();
		//	mDiceMtx.shrink_to_fit();
		//}
	}

	std::string mStageName;

	int mMapSizeWidth = 8;			// マップ横サイズ
	int mMapSizeHeight = 8;		// マップ縦サイズ

	// 床配置用マップ配列
	int mFloorMap[STAGESIZEMAX][STAGESIZEMAX] = {
		// 0:なし 1:床アリ 
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,0,1,1,1,1,1,1,1,
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

	int mDiceAlignCnt = 3;							// 揃った判定になる数
	int mTargetDiceType = 3;						// 揃えるサイコロの面
	int mStep = 5;									// サイコロの移動可能数

	Float3 mPlayerPos;								// プレイヤーの初期位置

	float mMapChipSize = 15.0f;

	int GetFloorMap(int _cnt)
	{
		return mFloorMap[_cnt%mMapSizeWidth][_cnt / mMapSizeWidth];
	}
};

