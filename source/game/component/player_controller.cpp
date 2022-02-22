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
	mpDirection.SetPtr(new Direction);
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
	mpDiceModel = dice;
	mpDiceModel->GetTransform()->SetScale((0.5f));
	mpDiceModel->Update();
	mpDiceModel->SetIsActive(false);

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
	mpDiceBg = diceBg;
	mpDiceBg->SetIsActive(false);

	transScreenToWorld(&mInfoDicePos, 150, 150, 0.9f);

	RemoveDiceUi();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
	mFoot = Foot::eFloor;
	(*mpDirection) = Direction::eDown;
	mpStageData = StageDataManager::GetInstance().GetCurrentStage();
	mpOwner->GetTransform()->SetPosition(Float3(mpStageData->mPlayerPos.x*DICE_SCALE, DICE_SCALE_HALF, -mpStageData->mPlayerPos.z*DICE_SCALE));
	mpOwner->GetTransform()->CreateWordMtx();

	// 最初の状態
	mStateNum = eMove;

	// 状態クラスをMapに登録
	mpStatesList[eMove].SetPtr(new PlayerState::Move);
	mpStatesList[ePush].SetPtr(new PlayerState::Push);
	mpStatesList[eRoll].SetPtr(new PlayerState::Roll);

	for (auto sts : mpStatesList)
	{
		sts.second->Start(this);
	}

	mpStatesList[mStateNum]->BeforeChange();
	RemoveDiceUi();
}

void PlayerController::Update()
{
	mpStatesList[mStateNum]->Exec();
	DiceManager::GetInstance()->SetPlayerPos(mpStatesList[mStateNum]->GetMapPos());

	if (!mIsDiceUiDraw)
		return;

	// 操作するサイコロが存在していれば
	if (mpStatesList[mStateNum]->GetOperationDice().IsExist())
	{
		XMFLOAT4X4 mtx = mpStatesList[mStateNum]->GetOperationDice()->GetTransform()->GetMtx();
		mtx._41 = mInfoDicePos.x;
		mtx._42 = mInfoDicePos.y;
		mtx._43 = mInfoDicePos.z;

		mpDiceModel->GetTransform()->SetWordMtx(mtx);
		mpDiceModel->GetTransform()->CreateScaleMtx();
		mpDiceModel->SetIsActive(true);
	}
	else
	{
		mpDiceModel->SetIsActive(false);
	}
}

void PlayerController::ImguiDraw()
{
	std::string str;
	str = DirectionStr[static_cast<int>(*mpDirection)];
	ImGui::Text(str.c_str());

	str = u8"現在のステート：　" + (PlayerStateStr[mStateNum]);
	ImGui::Text(str.c_str());

	if (ImGui::TreeNode("MapPos"))
	{
		str = "x" + std::to_string(mpStatesList[mStateNum]->GetMapPos().x);
		ImGui::Text(str.c_str());
		str = "y" + std::to_string(mpStatesList[mStateNum]->GetMapPos().y);
		ImGui::Text(str.c_str());
		str = "z" + std::to_string(mpStatesList[mStateNum]->GetMapPos().z);
		ImGui::Text(str.c_str());
		ImGui::TreePop();
	}
}

void PlayerController::SetDiceUi()
{
	transScreenToWorld(&mInfoDicePos, 150, 150, 0.9f);
	mIsDiceUiDraw = true;
	mpDiceBg->SetIsActive(true);
	mpStageData = StageDataManager::GetInstance().GetCurrentStage();

	for (auto sts : mpStatesList)
	{
		sts.second->Init();
	}
}

void PlayerController::RemoveDiceUi()
{
	mpDiceModel->SetIsActive(false);
	mpDiceBg->SetIsActive(false);
	mIsDiceUiDraw = false;
}

void PlayerController::StateInit(int _stateNum)
{
	mpStatesList[_stateNum]->Init();
}

void PlayerController::ChangeState(int _stateNum)
{
	mpStatesList[_stateNum]->SetOperationDice(mpStatesList[mStateNum]->GetOperationDice());
	mpStatesList[_stateNum]->SetMapPos(mpStatesList[mStateNum]->GetMapPos());
	mpStatesList[mStateNum]->AfterChange();
	mpStatesList[_stateNum]->BeforeChange();
	mStateNum = _stateNum;
}
