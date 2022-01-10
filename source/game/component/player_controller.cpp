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
	DiceManager::GetInstance()->SetPlayerPos(mStates[mStateNum]->GetMapPos());
}

void PlayerController::Draw()
{
}

void PlayerController::ImguiDraw()
{
	std::string str;
	str = DirectionStr[static_cast<int>(*mDirection)];
	ImGui::Text(str.c_str());

	if (ImGui::TreeNode("MapPos"))
	{
		str = "x" + std::to_string(mStates[mStateNum]->GetMapPos().x);
		ImGui::Text(str.c_str());
		str = "y" + std::to_string(mStates[mStateNum]->GetMapPos().y);
		ImGui::Text(str.c_str());
		str = "z" + std::to_string(mStates[mStateNum]->GetMapPos().z);
		ImGui::Text(str.c_str());
		ImGui::TreePop();
	}
}

void PlayerController::ChangeState(int _stateNum)
{
	mStates[_stateNum]->SetOperationDice(mStates[mStateNum]->GetOperationDice());
	mStates[_stateNum]->SetMapPos(mStates[mStateNum]->GetMapPos());
	mStates[mStateNum]->AfterChange();
	mStates[_stateNum]->BeforeChange();
	mStateNum = _stateNum;
}
