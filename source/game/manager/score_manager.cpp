#include "score_manager.h"

void ScoreManager::AddScore(int _diceNum, int _diceCnt, int _chain)
{
	// ダイスの目×ダイスの数×（チェイン数＋１）
	mScore += _diceNum * _diceCnt*(_chain + 1);
}
