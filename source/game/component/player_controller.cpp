#include "player_controller.h"
#include		"../state/allplayerstate.h"

using namespace Component;

PlayerController::PlayerController() :ComponentBase(("PlayerController"))
{
	mDirection.SetPtr(new Direction);
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
	mFoot = Foot::eFloor;
	(*mDirection) = Direction::eDown;
	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());
	mMapPos = (INT3(1, 0, 1));
	mOwner->GetTransform()->SetPosition(Float3(1 * DICE_SCALE, DICE_SCALE_HALF, -1 * DICE_SCALE));
	//mTransform.SetPosition(Float3(stageData.mPlayerPos.x*DICE_SCALE, DICE_SCALE_HALF, -stageData.mPlayerPos.z*DICE_SCALE));
	mOwner->GetTransform()->CreateMtx();

	// Å‰‚Ìó‘Ô
	mStateNum = eMove;

	// ó‘ÔƒNƒ‰ƒX‚ðMap‚É“o˜^
	mStates[eMove].SetPtr(new PlayerState::Move);
	mStates[ePush].SetPtr(new PlayerState::Push);
	mStates[eRoll].SetPtr(new PlayerState::Roll);

	for (auto sts : mStates)
	{
		sts.second->Start(this);
	}
}

void PlayerController::Update()
{
	mStates[mStateNum]->Exec();
	DiceManager::GetInstance()->SetPlayerPos(mMapPos);
}

void PlayerController::Draw()
{
}

void PlayerController::ImguiDraw()
{
}

void PlayerController::ChangeState(int _stateNum)
{
	mStates[_stateNum]->SetOperationDice(mStates[mStateNum]->GetOperationDice());
	mStates[mStateNum]->AfterChange();
	mStates[_stateNum]->BeforeChange();
	mStateNum = _stateNum;
}
