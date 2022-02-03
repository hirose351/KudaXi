#include "gamemode_controller.h"
#include "allgamemode.h"
#include "../gameobject/gameobject_utility.h"

using namespace Component;

Component::GameModeController::GameModeController() :ComponentBase(("GameModeController"))
{
	// 状態クラスをMapに登録
	mStates[eSelect].SetPtr(new GameModeState::Select);
	mStates[ePuzzle].SetPtr(new GameModeState::Puzzle);
	mStates[eEndless].SetPtr(new GameModeState::Endless);

	for (auto sts : mStates)
	{
		sts.second->Start(this);
	}
}

Component::GameModeController::~GameModeController()
{
}

void Component::GameModeController::Init(int _modeNum)
{
	// 最初の状態
	mModeNum = _modeNum;
	mStates[mModeNum]->BeforeChange();
}

void Component::GameModeController::Init()
{

	switch (SceneManager::GetInstance()->GetGameMode())
	{
	case ePuzzle:
		Init(eSelect);
		break;
	default:
		Init(SceneManager::GetInstance()->GetGameMode());
		break;
	}
}

void Component::GameModeController::Update()
{
	mStates[mModeNum]->Exec();
}

void Component::GameModeController::ImguiDraw()
{
}

void Component::GameModeController::ChangeMode(int _modeNum)
{
	mStates[mModeNum]->AfterChange();
	mStates[_modeNum]->BeforeChange();
	mModeNum = _modeNum;
}
