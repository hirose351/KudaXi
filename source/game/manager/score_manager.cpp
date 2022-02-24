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
	// ダイスの目×ダイスの数×（チェイン数＋１）
	mScore += _diceNum * _diceCnt*(_chain + 1);
}
