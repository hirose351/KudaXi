#include	"gamemode_state_puzzle.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/stagedata_manager.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/access_camera_eye.h"
#include	"../gameobject/access_camera_lookat.h"
#include	"../component/easing_component.h"
#include	"../component/player_controller.h"
#include	"../../application.h"
#include	"../component/map_pos_component.h"
#include	"../../system/util/XAudio2.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageStringUI
	Dix::sp<myUI::Image> stageString;
	stageString.SetPtr(new myUI::Image);
	stageString->GetTransform()->SetPositionXYZ(Float3(1280 / 2.0f, 50.0f, 0));
	stageString->GetTransform()->SetScale(Float3(200));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageString);
	Component::Quad2d* uiStageQuad = stageString->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/stage.png", XMFLOAT4(1, 1, 1, 1));
	uiStageQuad->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageString);

	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	stageNum->GetTransform()->SetPositionXYZ(Float3(1280 / 2.0f + 200, 50.0f, 0));
	stageNum->GetTransform()->SetScale(Float3(100));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	Component::Quad2d* uiStageNumQuad = stageNum->AddComponent<Component::Quad2d>();
	uiStageNumQuad->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	uiStageNumQuad->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageNum);
	mUiStageNum = stageNum;

	// StepStringUI
	Dix::sp<myUI::Image> stepString;
	stepString.SetPtr(new myUI::Image);
	stepString->GetTransform()->SetPositionXYZ(Float3(1140, 500, 0));
	stepString->GetTransform()->SetScale(Float3(250, 200, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepString);
	Component::Quad2d* uiStepQuad = stepString->AddComponent<Component::Quad2d>();
	uiStepQuad->SetInfo("assets/image/ui/step.png", XMFLOAT4(1, 1, 1, 1));
	uiStepQuad->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepString);

	// StepNumUI
	Dix::sp<myUI::Image> stepNum;
	stepNum.SetPtr(new myUI::Image);
	stepNum->GetTransform()->SetPositionXYZ(Float3(1150, 630, 0));
	stepNum->GetTransform()->SetScale(Float3(150));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepNum);
	Component::Quad2d* stepNumQuad = stepNum->AddComponent<Component::Quad2d>();
	stepNumQuad->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	stepNumQuad->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepNum);
	mUiStepNum = stepNum;

	// ClearOverUI
	Dix::sp<myUI::Image> clearOver;
	clearOver.SetPtr(new myUI::Image);
	clearOver->GetTransform()->SetPositionXYZ(Float3((float)Application::CLIENT_WIDTH / 2.0f, (float)Application::CLIENT_HEIGHT / 2.0f, 0));
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	Component::Quad2d* clearOverQuad = clearOver->AddComponent<Component::Quad2d>();
	clearOverQuad->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 2);
	clearOverQuad->SetOrderInLayer(1);
	mModeObjList.emplace_back(clearOver);
	mUiClearOver = clearOver;

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

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(false);
}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// カメラが動きを止めたらポーズ状態を解除
	if (!mIsStart)
	{
		if (mCameraEye->GetComponent<Component::Easing>()->GetEasingListCnt() == 0)
		{
			mIsStart = true;
			mIsClear = false;
			mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->SetDiceUi();
			DiceManager::GetInstance()->SetPuzzle();
		}
	}

	mStep = DiceManager::GetInstance()->GetStepCount();

	// ステップ番号更新
	mUiStepNum->GetComponent<Component::Quad2d>()->SetUvPos(INT2(mStep, 0));

	// ステップが0になった時
	if (mStep <= 0 && !mIsClear)
	{
		SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
		// クリアオーバー表示
		mUiClearOver->SetIsActive(true);
		mIsClear = true;
		// サイコロが全て揃っていれば
		if (DiceManager::GetInstance()->GetIsAllAligned())
		{
			// クリア
			mUiClearOver->GetComponent<Component::Quad2d>()->SetUvPos(INT2(1, 0));
			PlaySound(SOUND_LABEL_SE_CLEAR);
		}
		else
		{
			// オーバー
			mUiClearOver->GetComponent<Component::Quad2d>()->SetUvPos(INT2(0, 0));
			PlaySound(SOUND_LABEL_SE_RETRY);
		}
	}

	if (mIsCameraMove)
	{
		if (mCameraEye->GetComponent<Component::Easing>()->GetEasingListCnt() == 0)
			mHolder->ChangeMode(eSelect);
		return;
	}

	// 戻るを押されたときの処理
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		// 前のシーンがクリエイトならそのシーンに戻る
		if (SceneManager::GetInstance()->GetBeforeSceneKey() == "Create")
		{
			AfterChange();
			DiceManager::GetInstance()->Uninit();
			SceneManager::GetInstance()->SetNextScene(SceneManager::GetInstance()->GetBeforeSceneKey());
		}
		else
		{
			mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->RemoveDiceUi();
			mIsCameraMove = true;

			StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

			Float3 eye(data.mMapSizeWidth*data.mMapChipSize / 2.0f, 250, -197.5f - data.mMapSizeHeight*data.mMapChipSize / 2.0f);
			Float3 lookat(data.mMapSizeWidth*data.mMapChipSize / 2.0f, 0, -data.mMapSizeHeight*data.mMapChipSize / 2.0f);

			for (Dix::wp<GameObject> obj : mModeObjList)
			{
				obj->SetIsActive(false);
			}

			mCameraEye->ObjectInit();
			mCameraEye->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, eye, true);
			mCameraEye->SetIsActive(true);

			mCameraLookat->ObjectInit();
			mCameraLookat->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, lookat, true);
			mCameraLookat->SetIsActive(true);

			mHolder->SetIsSetCamera(true);

			// BGM設定
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_TITLE);
		}
	}
}

void Puzzle::BeforeChange()
{
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(true);
	}
	mIsCameraMove = false;
	mIsStart = false;

	// セレクトで選択されたステップ数を取得
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	mStep = data.mStep;

	// ステージ番号更新
	mUiStageNum->GetComponent<Component::Quad2d>()->SetUvPos(INT2(mHolder->GetSelectStage(), 0));
	// ステップ番号更新
	mUiStepNum->GetComponent<Component::Quad2d>()->SetUvPos(INT2(data.mStep, 0));

	// クリアオーバー非表示
	mUiClearOver->SetIsActive(false);

	// 前のシーンがクリエイトならDice生成
	if (SceneManager::GetInstance()->GetBeforeSceneKey() == "Create")
	{
		DiceManager::GetInstance()->DiceMapCreate(false);
		mHolder->GetPlayer()->GetComponent<Component::MapPos>()->SetMapPosMove(data.mPlayerPos);

		if (DiceManager::GetInstance()->GetDice(INT3(data.mPlayerPos.x, 0, data.mPlayerPos.z)) != NULL)
			mHolder->GetPlayer()->GetTransform()->SetPositionY(DICE_SCALE + mHolder->GetPlayer()->GetTransform()->scale.y);
		else
			mHolder->GetPlayer()->GetTransform()->SetPositionY(mHolder->GetPlayer()->GetTransform()->scale.y / 2.0f);
		mHolder->GetPlayer()->GetTransform()->angle = 0;
	}

	// サイコロの状態を通常に戻す
	DiceManager::GetInstance()->SetIsStepCount(true);
	DiceManager::GetInstance()->SetPuzzle();

	mCameraEye->ObjectInit();
	Float3 cameraVector(30.5f, 20.0f, -30.5f);
	Float3 pos;
	pos.x = cameraVector.x * data.mMapSizeWidth;
	pos.y = 70 + cameraVector.y * (data.mMapSizeWidth + data.mMapSizeHeight);
	pos.z = cameraVector.z * data.mMapSizeHeight;
	mCameraEye->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, pos, true);


	mCameraLookat->ObjectInit();
	Float3 cameraLookat;
	cameraLookat.x = data.mMapSizeWidth*DICE_SCALE_HALF;
	cameraLookat.y = 0;
	cameraLookat.z = -data.mMapSizeHeight*DICE_SCALE_HALF;
	mCameraLookat->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, cameraLookat, true);

	// BGM設定
	if (SceneManager::GetInstance()->GetBeforeSceneKey() == "Create")
	{
		StopSound(SOUND_LABEL_BGM_CREATE);
	}
	else
	{
		StopSound(SOUND_LABEL_BGM_TITLE);
	}
	PlaySound(SOUND_LABEL_BGM_GAME);
	PlaySound(SOUND_LABEL_SE_START);
}

void Puzzle::AfterChange()
{
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(false);
	}
	DiceManager::GetInstance()->SetIsStepCount(false);
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
}