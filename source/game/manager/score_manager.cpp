#include	"score_manager.h"
#define		DRAWCNT		(5)

void ScoreManager::Init()
{
	mScore = mDrawScore = mCrrentDrawCnt = 0;
}

void ScoreManager::Update()
{
	if (mScore == mDrawScore)
		return;
	if (mCrrentDrawCnt < DRAWCNT)
		mCrrentDrawCnt++;
	else
	{
		mDrawScore++;
		mCrrentDrawCnt = 0;
	}
}

void ScoreManager::AddScore(int _diceNum, int _diceCnt, int _chain)
{
	// �_�C�X�̖ځ~�_�C�X�̐��~�i�`�F�C�����{�P�j
	mScore += _diceNum * _diceCnt*(_chain + 1);
}
