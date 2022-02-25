#include	"gamemode_state_endless.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/pause_endless.h"
#include	"../gameobject/access_dice_endless_manager.h"
#include	"../gameobject/ui_image.h"
#include	"../gameobject/ui_score.h"
#include	"../component/player_controller.h"
#include	"../component/quad2d_component.h"
#include	"../../system/util/XAudio2.h"
#include	"../../application.h"

#define ENDLESSMAPSIZE (7)

using namespace GameModeState;

Endless::Endless()
{
	Dix::sp<myUI::PauseEndless> p;
	p.SetPtr(new myUI::PauseEndless);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(p);
	mpModeObjList.emplace_back(p);
	mpPauseBt = p;

	Dix::sp<DiceEndlessManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceEndlessManagerAccess);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dicemanager);
	mpModeObjList.emplace_back(dicemanager);

	// PauseUI
	Dix::sp<myUI::Image> pauseUi;
	pauseUi.SetPtr(new myUI::Image);
	pauseUi->GetTransform()->SetPositionXYZ(Float3(1170.0f, 80.0f, 0));
	pauseUi->GetTransform()->SetScale(Float3(150));
	Component::Quad2d* quadComponent = pauseUi->AddComponent<Component::Quad2d>();
	quadComponent->SetInfo("assets/image/ui/pause_ui.png", XMFLOAT4(1, 1, 1, 1));
	quadComponent->SetOrderInLayer(1);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(pauseUi);
	mpModeObjList.emplace_back(pauseUi);

	// OverUI
	Dix::sp<myUI::Image> overImage;
	overImage.SetPtr(new myUI::Image);
	overImage->GetTransform()->SetPositionXYZ(Float3(Application::CLIENT_WIDTH / 2.0f, Application::CLIENT_HEIGHT / 2.0f, 0));
	overImage->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(overImage);
	quadComponent = overImage->AddComponent<Component::Quad2d>();
	quadComponent->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 2);
	quadComponent->SetOrderInLayer(20);
	mpModeObjList.emplace_back(overImage);
	mpOverImage = overImage;

	// ScoreNumUI
	Dix::sp<myUI::Score> scoreNum;
	scoreNum.SetPtr(new myUI::Score);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(scoreNum);
	mpModeObjList.emplace_back(scoreNum);
	mpScoreNum = scoreNum;

	// OverUI
	Dix::sp<myUI::Image> scoreString;
	scoreString.SetPtr(new myUI::Image);
	scoreString->GetTransform()->SetPositionXYZ(Float3(80, 30, 0));
	scoreString->GetTransform()->SetScale(Float3(100));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(scoreString);
	quadComponent = scoreString->AddComponent<Component::Quad2d>();
	quadComponent->SetInfo("assets/image/ui/score_string.png", XMFLOAT4(1, 1, 1, 1));
	quadComponent->SetOrderInLayer(100);
	mpModeObjList.emplace_back(scoreString);

	for (Dix::wp<GameObject> obj : mpModeObjList)
		obj->SetIsActive(false);
}

Endless::~Endless()
{
}

void Endless::Start(Component::GameModeController * _c)
{
	mHolder = _c;
}

void Endless::Exec()
{
	// ñﬂÇÈÇâüÇ≥ÇÍÇΩÇ∆Ç´ÇÃèàóù
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		AfterChange();
		SceneManager::GetInstance()->SetNextScene("Mode");
	}

	if (mIsOver)
		return;

	if (DiceManager::GetInstance()->GetEndlessIsOver())
	{
		// É|Å[ÉYÇ…Ç∑ÇÈ
		SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
		mpOverImage->SetIsActive(true);
		PlaySound(SOUND_LABEL_SE_RETRY);
		mIsOver = true;
	}
}

void Endless::BeforeChange()
{
	mpPauseBt->SetParent(mHolder->GetOwner());

	StageDataManager::GetInstance().SetCurrentStage("endless");
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();
	stageData->mMapSizeWidth = stageData->mMapSizeHeight = ENDLESSMAPSIZE;

	Float3 cameraLookat;
	cameraLookat.x = stageData->mMapSizeWidth*DICE_SCALE_HALF;
	cameraLookat.y = { 0 };
	cameraLookat.z = { -stageData->mMapSizeHeight*DICE_SCALE_HALF };
	Camera::GetInstance()->SetLookat(cameraLookat);
	Camera::GetInstance()->SetEye(Float3(140, 130, -170));
	Camera::GetInstance()->CreateCameraMatrix();

	DiceManager::GetInstance()->EndlessInit();
	mHolder->GetStage()->ObjectInit();
	mHolder->GetPlayer()->Init();

	for (Dix::wp<GameObject> obj : mpModeObjList)
	{
		obj->SetIsActive(true);
	}

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->SetDiceUi();
	mpOverImage->SetIsActive(false);
	mIsOver = false;
	// BGMê›íË
	StopSound(SOUND_LABEL_BGM_TITLE);
	PlaySound(SOUND_LABEL_BGM_GAME);
}

void Endless::AfterChange()
{
	DiceManager::GetInstance()->Uninit();
	for (Dix::wp<GameObject> obj : mpModeObjList)
	{
		obj->SetIsActive(false);
	}
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);

	// BGMê›íË
	StopSound(SOUND_LABEL_BGM_GAME);
	PlaySound(SOUND_LABEL_BGM_TITLE);
}
