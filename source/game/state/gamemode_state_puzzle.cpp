#include	"gamemode_state_puzzle.h"
#include	"../manager/stagedata_manager.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/access_camera_eye.h"
#include	"../gameobject/access_camera_lookat.h"
#include	"../gameobject/ui_image.h"
#include	"../component/easing_component.h"
#include	"../component/number_component.h"
#include	"../component/player_controller.h"
#include	"../component/map_pos_component.h"
#include	"../../application.h"
#include	"../../system/util/XAudio2.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageStringUI
	Dix::sp<myUI::Image> stageString;
	stageString.SetPtr(new myUI::Image);
	stageString->GetTransform()->SetPositionXYZ(Float3(Application::CLIENT_WIDTH / 2.0f, 50.0f, 0));
	stageString->GetTransform()->SetScale(Float3(200));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageString);
	Component::Quad2d* uiStageQuad = stageString->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/stage.png", XMFLOAT4(1, 1, 1, 1));
	uiStageQuad->SetOrderInLayer(1);
	mpModeObjList.emplace_back(stageString);

	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	stageNum->GetTransform()->SetPositionXYZ(Float3(Application::CLIENT_WIDTH / 2.0f + 200, 50.0f, 0));
	stageNum->GetTransform()->SetScale(Float3(100));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	Component::Quad2d* uiStageNumQuad = stageNum->AddComponent<Component::Quad2d>();
	uiStageNumQuad->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	uiStageNumQuad->SetOrderInLayer(1);
	stageNum->AddComponent<Component::Number>();
	mpModeObjList.emplace_back(stageNum);
	mpUiStageNum = stageNum;

	// StepStringUI
	Dix::sp<myUI::Image> stepString;
	stepString.SetPtr(new myUI::Image);
	stepString->GetTransform()->SetPositionXYZ(Float3(1150, 500, 0));
	stepString->GetTransform()->SetScale(Float3(250, 200, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepString);
	Component::Quad2d* uiStepQuad = stepString->AddComponent<Component::Quad2d>();
	uiStepQuad->SetInfo("assets/image/ui/step.png", XMFLOAT4(1, 1, 1, 1));
	uiStepQuad->SetOrderInLayer(1);
	mpModeObjList.emplace_back(stepString);

	// StepNumUI
	Dix::sp<myUI::Image> stepNum;
	stepNum.SetPtr(new myUI::Image);
	stepNum->GetTransform()->SetPositionXYZ(Float3(1140, 630, 0));
	stepNum->GetTransform()->SetScale(Float3(150));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepNum);
	stepNum->AddComponent<Component::Number>();
	Component::Quad2d* stepNumQuad = stepNum->AddComponent<Component::Quad2d>();
	stepNumQuad->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	stepNumQuad->SetOrderInLayer(1);
	mpModeObjList.emplace_back(stepNum);
	mpUiStepNum = stepNum;

	// ClearOverUI
	Dix::sp<myUI::Image> clearOver;
	clearOver.SetPtr(new myUI::Image);
	clearOver->GetTransform()->SetPositionXYZ(Float3(Application::CLIENT_WIDTH / 2.0f, Application::CLIENT_HEIGHT / 2.0f, 0));
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	Component::Quad2d* clearOverQuad = clearOver->AddComponent<Component::Quad2d>();
	clearOverQuad->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 2);
	clearOverQuad->SetOrderInLayer(100);
	mpModeObjList.emplace_back(clearOver);
	mpUiClearOver = clearOver;

	// cameraEye
	Dix::sp<CameraEyeAccess> cameraEye;
	cameraEye.SetPtr(new CameraEyeAccess);
	mpCameraEye = cameraEye;
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(cameraEye);
	mpModeObjList.emplace_back(cameraEye);

	// cameralookat
	Dix::sp<CameraAccessLookat> cameralookat;
	cameralookat.SetPtr(new CameraAccessLookat);
	mpCameraLookat = cameralookat;
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(cameralookat);
	mpModeObjList.emplace_back(cameralookat);

	/// Todo:?????X?e?[?W???A?X?e?[?W?Z???N?g???A???????x

	for (Dix::wp<GameObject> obj : mpModeObjList)
		obj->SetIsActive(false);
}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// ?J????????????????????????
	if (mIsCameraMove)
	{
		if (mpCameraEye->GetComponent<Component::Easing>()->GetEasingListCnt() == 0)
		{
			for (Dix::wp<GameObject> obj : mpModeObjList)
				obj->Update();

			mHolder->ChangeMode(eSelect);
			// BGM????
			StopSound(SOUND_LABEL_BGM_GAME);
			PlaySound(SOUND_LABEL_BGM_TITLE);
		}
		return;		// ?????X?V??????????
	}

	// ?J?????????????~???????|?[?Y??????????
	if (!mIsStart)
	{
		if (mpCameraEye->GetComponent<Component::Easing>()->GetEasingListCnt() == 0)
		{
			mIsStart = true;
			mIsClear = false;
			mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->SetDiceUi();
			DiceManager::GetInstance()->SetPuzzle();
		}
	}

	mStep = DiceManager::GetInstance()->GetStepCount();

	// ?X?e?b?v?????X?V
	mpUiStepNum->GetComponent<Component::Number>()->SetNum(mStep, -30);

	// ?X?e?b?v??0??????????
	if (mStep <= 0 && !mIsClear)
	{
		// ?|?[?Y???????O???X?e?b?v???I?u?W?F?N?g?X?V
		mpUiStepNum->Update();
		// ?|?[?Y??????
		SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
		// ?N???A?I?[?o?[?\??
		mpUiClearOver->SetIsActive(true);
		mIsClear = true;
		// ?T?C?R?????S??????????????
		if (DiceManager::GetInstance()->GetIsAllAligned())
		{
			// ?N???A
			mpUiClearOver->GetComponent<Component::Quad2d>()->SetUvPos(INT2(1, 0));
			PlaySound(SOUND_LABEL_SE_CLEAR);
		}
		else
		{
			// ?I?[?o?[
			mpUiClearOver->GetComponent<Component::Quad2d>()->SetUvPos(INT2(0, 0));
			PlaySound(SOUND_LABEL_SE_RETRY);
		}
	}

	// ????????????????????????
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		// ?O???V?[?????N???G?C?g?????????V?[????????
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

			mpCameraEye->ObjectInit();
			mpCameraEye->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, eye, true);
			mpCameraEye->SetIsActive(true);

			mpCameraLookat->ObjectInit();
			mpCameraLookat->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, lookat, true);
			mpCameraLookat->SetIsActive(true);

			mHolder->SetIsSetCamera(true);
		}
	}
}

void Puzzle::BeforeChange()
{
	for (Dix::wp<GameObject> obj : mpModeObjList)
	{
		obj->SetIsActive(true);
	}
	mIsCameraMove = false;
	mIsStart = false;

	// ?Z???N?g???I?????????X?e?b?v????????
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	mStep = data.mStep;

	// ?X?e?[?W?????X?V
	mpUiStageNum->GetComponent<Component::Number>()->SetNum(mHolder->GetSelectStage(), -30);
	// ?X?e?b?v?????X?V
	mpUiStepNum->GetComponent<Component::Number>()->SetNum(data.mStep);
	// ?N???A?I?[?o?[???\??
	mpUiClearOver->SetIsActive(false);

	mHolder->GetPlayer()->GetTransform()->angle = 0;
	mHolder->GetPlayer()->GetTransform()->move = 0;

	// ?O???V?[?????N???G?C?g????Dice????
	if (SceneManager::GetInstance()->GetBeforeSceneKey() == "Create")
	{
		DiceManager::GetInstance()->DiceMapCreate(false);
		mHolder->GetStage()->ObjectInit();
		mHolder->GetPlayer()->GetComponent<Component::MapPos>()->SetMapPosMove(data.mPlayerPos);

		if (DiceManager::GetInstance()->GetDice(INT3(data.mPlayerPos.x, 0, data.mPlayerPos.z)) != NULL)
			mHolder->GetPlayer()->GetTransform()->SetPositionY(DICE_SCALE + mHolder->GetPlayer()->GetTransform()->scale.y);
		else
			mHolder->GetPlayer()->GetTransform()->SetPositionY(mHolder->GetPlayer()->GetTransform()->scale.y / 2.0f);
		mHolder->GetPlayer()->GetTransform()->angle = 0;
	}

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->StateInit(eMove);

	// ?T?C?R????????????????????
	DiceManager::GetInstance()->SetIsStepCount(true);
	DiceManager::GetInstance()->SetPuzzle();

	mpCameraEye->ObjectInit();
	Float3 pos, cameraVector(30.5f, 20.0f, -30.5f);
	pos.x = cameraVector.x * data.mMapSizeWidth;
	pos.y = 70 + cameraVector.y * (data.mMapSizeWidth + data.mMapSizeHeight);
	pos.z = cameraVector.z * data.mMapSizeHeight;
	mpCameraEye->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, pos, true);


	mpCameraLookat->ObjectInit();
	Float3 cameraLookat;
	cameraLookat.x = data.mMapSizeWidth*DICE_SCALE_HALF;
	cameraLookat.y = 0;
	cameraLookat.z = -data.mMapSizeHeight*DICE_SCALE_HALF;
	mpCameraLookat->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 50.0f, 0.0f, 0, cameraLookat, true);

	// BGM????
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
	for (Dix::wp<GameObject> obj : mpModeObjList)
	{
		obj->SetIsActive(false);
	}
	DiceManager::GetInstance()->SetIsStepCount(false);
}