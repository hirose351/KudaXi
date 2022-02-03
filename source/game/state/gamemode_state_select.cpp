#include	"gamemode_state_select.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"

using namespace GameModeState;

Select::Select()
{

	Dix::sp<myUI::ButtonGroup> b;
	b.SetPtr(new myUI::ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(15, 500, 0));
	b->SetInitState("assets/image/ui/number.png", 10, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(0, 0), XMFLOAT2(50, 50), XMFLOAT2(70, 70), ButtonArrangement::eHorizontal, StartPoint::eLeftUp, true, 10);
	b->SetIsActive(false);
	mButton = b;
	mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(b);


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
		if (mStageNum / 10 > 0)
		{
			mStr = "puzzle/" + std::to_string(mStageNum);
		}
		else
		{
			mStr = "puzzle/0" + std::to_string(mStageNum);
		}

		StageDataManager::GetInstance().SetCurrentStage(mStr);
		Dix::wp<StageData> stageData;
		stageData = StageDataManager::GetInstance().GetCurrentStage();

		DiceManager::GetInstance()->DiceMapCreate(false);

		Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -57.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->CreateCameraMatrix();
	}



	// 押された瞬間の処理
	InputDirection i = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));

	// ハイライト&大きさ変更されてる番号の変更処理



	// 決定を押されたときの処理（ステージはそのままでカメラを動かす感じ）
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eClick)))
	{

	}

}

void Select::BeforeChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
	// カメラの位置変えよう
	if (mStageNum / 10 > 0)
	{
		mStr = "puzzle/" + std::to_string(mStageNum);
	}
	else
	{
		mStr = "puzzle/0" + std::to_string(mStageNum);
	}

	DiceManager::GetInstance()->DiceMapCreate(false);

	StageDataManager::GetInstance().SetCurrentStage(mStr);
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();

	Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -57.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->CreateCameraMatrix();

	mButton->SetIsActive(true);
}

void Select::AfterChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
	mButton->SetIsActive(false);
}
