#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../gameobject/gameobject.h"

class ScoreManager : public Uncopyable
{
private:
	unsigned int mScore;			// 総スコア

public:
	static ScoreManager* GetInstance() {
		static ScoreManager Instance;
		return &Instance;
	}

	int GetScore() {
		return mScore;
	}

	// スコア加算(Diceの目,Diceの個数,チェイン数)
	void AddScore(int _diceNum, int _diceCnt, int _chain);
};

