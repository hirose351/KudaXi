#include "gamemode_controller.h"

void GameModeController::Init(int _modeNum)
{
	// 最初の状態
	mModeNum = _modeNum;

	//// 状態クラスをMapに登録
	//mStates[eSelect].SetPtr(new PlayerState::Move);
	//mStates[ePuzzle].SetPtr(new PlayerState::Push);
	//mStates[eEndless].SetPtr(new PlayerState::Roll);
}

void GameModeController::Update()
{
	mStates[mModeNum]->Exec();
}

void GameModeController::ChangeMode(int _modeNum)
{
	mStates[_modeNum]->AfterChange();
	mStates[_modeNum]->BeforeChange();
	mModeNum = _modeNum;
}
