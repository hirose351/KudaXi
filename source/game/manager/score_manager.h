#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../gameobject/gameobject.h"

class ScoreManager : public Uncopyable
{
private:
	unsigned int mScore = 0;			// 総スコア
	unsigned int mDrawScore = 0;		// 表示スコア
	unsigned int mCrrentDrawCnt = 0;	// 今の回転回数
	bool mIsDrawCntUpdate = false;		// 今の回転回数

public:
	static ScoreManager* GetInstance() {
		static ScoreManager Instance;
		return &Instance;
	}

	int GetScore() { return mScore; }
	int GetDrawScore() { return mDrawScore; }
	void Init();
	void Update();

	// スコア加算(Diceの目,Diceの個数,チェイン数)
	void AddScore(int _diceNum, int _diceCnt, int _chain);
};

