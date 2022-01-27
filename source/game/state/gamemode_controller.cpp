#include "gamemode_controller.h"
#include "allgamemode.h"

GameModeController::GameModeController() :ComponentBase(("GameModeController"))
{
}

GameModeController::~GameModeController()
{
}

void GameModeController::Init(int _modeNum)
{
	// 最初の状態
	mModeNum = _modeNum;

	// 状態クラスをMapに登録
	mStates[eSelect].SetPtr(new GameModeState::Select);
	mStates[ePuzzle].SetPtr(new GameModeState::Puzzle);
	mStates[eEndless].SetPtr(new GameModeState::Endless);

	for (auto sts : mStates)
	{
		sts.second->Start(this);
	}
}

void GameModeController::Init()
{
}

void GameModeController::Update()
{
	mStates[mModeNum]->Exec();
}

void GameModeController::ImguiDraw()
{
}

void GameModeController::ChangeMode(int _modeNum)
{
	mStates[_modeNum]->AfterChange();
	mStates[_modeNum]->BeforeChange();
	mModeNum = _modeNum;
}
