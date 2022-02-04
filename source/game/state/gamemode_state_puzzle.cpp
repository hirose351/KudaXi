#include	"gamemode_state_puzzle.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/stagedata_manager.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/access_camera_eye.h"
#include	"../gameobject/access_camera_lookat.h"
#include	"../component/easing_component.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageStringUI
	Dix::sp<myUI::Image> stageString;
	stageString.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageString);
	mUiStage = stageString->AddComponent<Component::Quad2d>();
	mUiStage->SetInfo("assets/image/ui/stage.png", XMFLOAT4(1, 1, 1, 1));
	mUiStage->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageString);

	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	mUiStageNum = stageNum->AddComponent<Component::Quad2d>();
	mUiStageNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStageNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageNum);

	// StepStringUI
	Dix::sp<myUI::Image> stepString;
	stepString.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepString);
	mUiStep = stepString->AddComponent<Component::Quad2d>();
	mUiStep->SetInfo("assets/image/ui/step.png", XMFLOAT4(1, 1, 1, 1));
	mUiStep->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepString);

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
	clearOver.SetPtr(new myUI::Image);
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	mUiClearOver = clearOver->AddComponent<Component::Quad2d>();
	mUiClearOver->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 1, 2);
	mUiClearOver->SetOrderInLayer(1);
	mModeObjList.emplace_back(clearOver);

	// cameraEye
	Dix::sp<CameraEyeAccess> cameraEye;
	cameraEye.SetPtr(new CameraEyeAccess);
	mCameraEye = cameraEye;
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(cameraEye);
	mModeObjList.emplace_back(cameraEye);

	// cameralookat
	Dix::sp<CameraAccessLookat> cameralookat;
	cameralookat.SetPtr(new CameraAccessLookat);
	mCameraLookat = cameralookat;
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(cameralookat);
	mModeObjList.emplace_back(cameralookat);

	// 次のステージへ、ステージセレクトへ、もう一度

	mUiStage->SetIsDraw(false);
	mUiStageNum->SetIsDraw(false);
	mUiStep->SetIsDraw(false);
	mUiStepNum->SetIsDraw(false);
	mUiClearOver->SetIsDraw(false);
	mCameraEye->SetIsActive(false);
	mCameraLookat->SetIsActive(false);
}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// プレイヤーorサイコロの行動によってステップを減らす

	// 戻るを押されたときの処理
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		mHolder->ChangeMode(eSelect);
	}
}

void Puzzle::BeforeChange()
{
	//for (Dix::wp<GameObject> obj : mModeObjList)
	//{
	//	obj->SetIsActive(true);
	//}

	mUiStage->SetIsDraw(true);
	mUiStageNum->SetIsDraw(true);
	mUiStep->SetIsDraw(true);
	mUiStepNum->SetIsDraw(true);
	// セレクトで選択されたステップ数を取得
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	mStep = data.mStep;
	// ステージ番号更新
	mUiStageNum->SetUvPos(INT2(mHolder->GetSelectStage(), 0));
	// ステップ番号更新
	mUiStepNum->SetUvPos((data.mStep, 0));
	// クリアオーバー非表示
	mUiClearOver->SetIsDraw(false);
	// サイコロの状態を通常に戻す
	DiceManager::GetInstance()->SetPuzzle();

	mCameraEye->SetIsActive(true);
	mCameraLookat->SetIsActive(true);


	mCameraEye->ObjectInit();
	Float3 cameraVector(20.5f, 4.4f, -27.5f);
	Float3 pos(data.mMapSizeWidth*data.mMapChipSize / 2.0f, 250, -197.5f - data.mMapSizeHeight*data.mMapChipSize / 2.0f);
	pos.x = cameraVector.x * data.mMapSizeWidth;
	pos.y = 70 + cameraVector.y * (data.mMapSizeWidth + data.mMapSizeHeight);
	pos.z = cameraVector.z * data.mMapSizeHeight;
	mCameraEye->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, pos, true);


	mCameraLookat->ObjectInit();
	Float3 cameraLookat;
	cameraLookat.x = data.mMapSizeWidth*DICE_SCALE_HALF;
	cameraLookat.y = 0;
	cameraLookat.z = -data.mMapSizeHeight*DICE_SCALE_HALF;
	//Camera::GetInstance()->SetLookat(cameraLookat);
	mCameraLookat->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, cameraLookat, true);
}

void Puzzle::AfterChange()
{
	mUiStage->SetIsDraw(false);
	mUiStageNum->SetIsDraw(false);
	mUiStep->SetIsDraw(false);
	mUiStepNum->SetIsDraw(false);
	mUiClearOver->SetIsDraw(false);
	mCameraEye->SetIsActive(false);
	mCameraLookat->SetIsActive(false);
}
