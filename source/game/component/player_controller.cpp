#include		"player_controller.h"
#include		"../state/allplayerstate.h"
#include		"model_component.h"
#include		"quad2d_component.h"
#include		"animation2d_component.h"
#include		"../gameobject/ui_image.h"

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
	dice->GetComponent<Component::Model>()->SetOrderInLayer(5);
	mDiceModel = dice;
	mDiceModel->GetTransform()->SetScale((0.5f));
	mDiceModel->Update();
	mDiceModel->SetIsActive(false);

	Dix::sp<myUI::Image> diceBg;
	diceBg.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(diceBg);
	Component::Quad2d* bgQuad = diceBg->AddComponent<Component::Quad2d>();
	bgQuad->SetInfo("", XMFLOAT4(1, 1, 1, 0.8f));
	bgQuad->SetDrawType(DrawType::eNoTex);
	bgQuad->SetOrderInLayer(1);
	diceBg->GetTransform()->SetScale((188));
	diceBg->GetTransform()->SetPosition(Float3(150));
	diceBg->GetTransform()->CreateWordMtx();
	mDiceBg = diceBg;
	mDiceBg->SetIsActive(false);

	transScreenToWorld(&mInfoDicePos, 150, 150, 0.9f);

	RemoveDiceUi();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
	mFoot = Foot::eFloor;
	(*mDirection) = Direction::eDown;
	mStageData = StageDataManager::GetInstance().GetCurrentStage();
	//mOwner->GetTransform()->SetPosition(Float3(1 * DICE_SCALE, DICE_SCALE_HALF, -1 * DICE_SCALE));
	mOwner->GetTransform()->SetPosition(Float3(mStageData->mPlayerPos.x*DICE_SCALE, DICE_SCALE_HALF, -mStageData->mPlayerPos.z*DICE_SCALE));
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

	mStates[mStateNum]->BeforeChange();
	RemoveDiceUi();
}

void PlayerController::Update()
{
	mStates[mStateNum]->Exec();
	DiceManager::GetInstance()->SetPlayerPos(mStates[mStateNum]->GetMapPos());

	if (!mIsDiceUiDraw)
		return;

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

	str = u8"現在のステート：　" + (PlayerStateStr[mStateNum]);
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

void PlayerController::SetDiceUi()
{
	transScreenToWorld(&mInfoDicePos, 150, 150, 0.9f);
	mIsDiceUiDraw = true;
	mDiceBg->SetIsActive(true);
	mStageData = StageDataManager::GetInstance().GetCurrentStage();

	for (auto sts : mStates)
	{
		sts.second->Init();
	}
}

void PlayerController::RemoveDiceUi()
{
	mDiceModel->SetIsActive(false);
	mDiceBg->SetIsActive(false);
	mIsDiceUiDraw = false;
}

void PlayerController::StateInit(int _stateNum)
{
	mStates[_stateNum]->Init();
}

void PlayerController::ChangeState(int _stateNum)
{
	mStates[_stateNum]->SetOperationDice(mStates[mStateNum]->GetOperationDice());
	mStates[_stateNum]->SetMapPos(mStates[mStateNum]->GetMapPos());
	mStates[mStateNum]->AfterChange();
	mStates[_stateNum]->BeforeChange();
	mStateNum = _stateNum;
}
