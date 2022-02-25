#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../gameobject/gameobject.h"

class ScoreManager : public Uncopyable
{
private:
	unsigned int mScore = 0;			// ���X�R�A
	unsigned int mDrawScore = 0;		// �\���X�R�A
	unsigned int mCrrentDrawCnt = 0;	// ���̉�]��
	bool mIsDrawCntUpdate = false;		// ���̉�]��

public:
	static ScoreManager* GetInstance() {
		static ScoreManager Instance;
		return &Instance;
	}

	int GetScore() { return mScore; }
	int GetDrawScore() { return mDrawScore; }
	void Init();
	void Update();

	// �X�R�A���Z(Dice�̖�,Dice�̌�,�`�F�C����)
	void AddScore(int _diceNum, int _diceCnt, int _chain);
};

