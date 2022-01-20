#include		"player_controller.h"
#include		"../state/allplayerstate.h"
#include		"model_component.h"

using namespace Component;

std::string PlayerStateStr[] =
{ "Move", "Push", "Roll" };

PlayerController::PlayerController() :ComponentBase(("PlayerController"))
{
	mDirection.SetPtr(new Direction);
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/dice/Dice.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/dice/");
	if (!sts)
	{
		MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
	}
	Dix::sp<GameObject> dice;
	dice.SetPtr(new GameObject("", ObjectType::eObstracle, false));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
	dice->AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
	mDiceModel = dice;
	mDiceModel->GetTransform()->SetScale((0.5f));
	mDiceModel->SetIsActive(false);

	transScreenToWorld(&mInfoDicePos, 200, 200, 0.9f);
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
	mOwner->GetTransform()->CreateWordMtx();

	// 最初の状態
	mStateNum = eMove;

	// 状態クラスをMapに登録
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

	// 操作するサイコロが存在していれば
	if (mStates[mStateNum]->GetOperationDice().IsExist())
	{
		XMFLOAT4X4 mtx = mStates[mStateNum]->GetOperationDice()->GetTransform()->GetMtx();
		mtx._41 = mInfoDicePos.x;
		mtx._42 = mInfoDicePos.y;
		mtx._43 = mInfoDicePos.z;

		mDiceModel->GetTransform()->SetWordMtx(mtx);
		mDiceModel->GetTransform()->CreateScaleMtx();
		mDiceModel->SetIsActive(true);
	}
	else
	{
		mDiceModel->SetIsActive(false);
	}
}

void PlayerController::ImguiDraw()
{
	std::string str;
	str = DirectionStr[static_cast<int>(*mDirection)];
	ImGui::Text(str.c_str());

	str = "現在のステート：　" + (PlayerStateStr[mStateNum]);
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
