#include	"gamemode_state_select.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../component/player_controller.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"
#include	"../component/map_pos_component.h"

using namespace GameModeState;

void Select::SetStage()
{
	if (mStageNum / 10 > 0)
		mStr = "puzzle/" + std::to_string(mStageNum);
	else
		mStr = "puzzle/0" + std::to_string(mStageNum);

	StageDataManager::GetInstance().SetCurrentStage(mStr);
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();

	DiceManager::GetInstance()->DiceMapCreate(false);
	mHolder->GetStage()->Init();
	mHolder->GetPlayer()->GetComponent<Component::MapPos>()->SetMapPosMove(stageData->mPlayerPos);
	if (DiceManager::GetInstance()->GetDice(INT3(stageData->mPlayerPos.x, 0, stageData->mPlayerPos.z)) != NULL)
		mHolder->GetPlayer()->GetTransform()->SetPositionY(DICE_SCALE + mHolder->GetPlayer()->GetTransform()->scale.y);
	else
		mHolder->GetPlayer()->GetTransform()->SetPositionY(mHolder->GetPlayer()->GetTransform()->scale.y / 2.0f);

	Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->CreateCameraMatrix();
}

Select::Select()
{
	// ボタン
	Dix::sp<myUI::ButtonGroup> b;
	b.SetPtr(new myUI::ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(150, 500, 0));
	b->SetInitState("assets/image/ui/number.png", 10, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(50, 0), XMFLOAT2(50, 50), XMFLOAT2(70, 70), ButtonArrangement::eHorizontal, StartPoint::eLeftUp, true, 2);
	mButton = b; mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(b);
	mModeObjList.emplace_back(mButton);

	// 文字
	Dix::sp<myUI::Image> selectString;
	selectString.SetPtr(new myUI::Image);
	selectString->GetTransform()->SetScale(Float3(400));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(selectString);
	Component::Quad2d* uiStageQuad = selectString->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/Select.png", XMFLOAT4(1, 1, 1, 1));
	uiStageQuad->SetOrderInLayer(1);
	selectString->GetTransform()->SetPositionXYZ(Float3(1280 / 2.0f, 100.0f, 0));
	mModeObjList.emplace_back(selectString);

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(false);
}

Select::~Select()
{
}

void Select::Exec()
{
	// 入力に対する処理（入力されていたらステージ、プレイヤー、サイコロ更新）

	// 選択されているステージ番号が変更されたらステージ情報を更新する
	if (mStageNum != mButton->GetSelectNum() + 1)
	{
		mStageNum = mButton->GetSelectNum() + 1;
		SetStage();
	}

	// 決定を押されたときの処理（ステージはそのままでカメラを動かす感じ）
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eClick)))
	{
		mHolder->SetSelectStage(mButton->GetSelectNum() + 1);
		mHolder->ChangeMode(ePuzzle);
	}
	// 戻るを押されたときの処理
	else if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		AfterChange();
		SceneManager::GetInstance()->SetNextScene("Mode");
	}
}

void Select::BeforeChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
	SetStage();

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(true);

	mButton->SetInitSelectNum(mStageNum - 1);

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->RemoveDiceUi();
}

void Select::AfterChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(false);
}
