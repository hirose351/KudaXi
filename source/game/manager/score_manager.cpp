#include "score_manager.h"

void ScoreManager::AddScore(int _diceNum, int _diceCnt, int _chain)
{
	// �_�C�X�̖ځ~�_�C�X�̐��~�i�`�F�C�����{�P�j
	mScore += _diceNum * _diceCnt*(_chain + 1);
}
