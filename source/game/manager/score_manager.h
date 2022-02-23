#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../gameobject/gameobject.h"

class ScoreManager : public Uncopyable
{
private:
	unsigned int mScore;			// ���X�R�A

public:
	static ScoreManager* GetInstance() {
		static ScoreManager Instance;
		return &Instance;
	}

	int GetScore() {
		return mScore;
	}

	// �X�R�A���Z(Dice�̖�,Dice�̌�,�`�F�C����)
	void AddScore(int _diceNum, int _diceCnt, int _chain);
};

