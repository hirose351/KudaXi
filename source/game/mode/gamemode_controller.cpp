#include "gamemode_controller.h"

void GameModeController::Init(int _modeNum)
{
	// Å‰‚Ìó‘Ô
	mModeNum = _modeNum;

	//// ó‘ÔƒNƒ‰ƒX‚ðMap‚É“o˜^
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
