#include	"gamemode_state_puzzle.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/stagedata_manager.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	mUiStageNum = stageNum->AddComponent<Component::Quad2d>();
	mUiStageNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStageNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageNum);

	// StepNumUI
	Dix::sp<myUI::Image> stepNum;
	stepNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepNum);
	mUiStepNum = stepNum->AddComponent<Component::Quad2d>();
	mUiStepNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStepNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepNum);

	// ClearOverUI
	Dix::sp<myUI::Image> clearOver;
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	clearOver.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	mUiClearOver = clearOver->AddComponent<Component::Quad2d>();
	mUiClearOver->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 1, 2);
	mUiClearOver->SetOrderInLayer(1);
	mModeObjList.emplace_back(clearOver);

	// 次のステージへ、ステージセレクトへ、もう一度


}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// プレイヤーorサイコロの行動によってステップを減らす

}

void Puzzle::BeforeChange()
{
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(true);
	}

	// セレクトで選択されたステップ数を取得
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	// ステージ番号更新
	//mStageNum->SetUvPos();
	// ステップ番号更新
	mUiStepNum->SetUvPos((data.mStep, 0));
	// クリアオーバー非表示
	mUiClearOver->SetIsDraw(false);
}

void Puzzle::AfterChange()
{

}
