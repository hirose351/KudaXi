#include	"gamemode_state_endless.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/pause_endless.h"
#include	"../gameobject/access_dice_manager.h"
#include	"../component/player_controller.h"
#include	"../../system/util/XAudio2.h"

#define ENDLESSMAPSIZE (7)

using namespace GameModeState;

Endless::Endless()
{
	Dix::sp<myUI::PauseEndless> p;
	p.SetPtr(new myUI::PauseEndless);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(p);
	p->SetIsActive(false);
	mModeObjList.emplace_back(p);

	Dix::sp<DiceManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceManagerAccess);
	dicemanager->SetIsActive(false);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dicemanager);
	dicemanager->SetIsActive(false);
	mModeObjList.emplace_back(dicemanager);
}

Endless::~Endless()
{
}

void Endless::Exec()
{
	// ñﬂÇÈÇâüÇ≥ÇÍÇΩÇ∆Ç´ÇÃèàóù
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		AfterChange();
		SceneManager::GetInstance()->SetNextScene("Mode");
	}
}

void Endless::BeforeChange()
{
	StageDataManager::GetInstance().SetCurrentStage("endless");
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();
	stageData->mMapSizeWidth = stageData->mMapSizeHeight = ENDLESSMAPSIZE;

	DiceManager::GetInstance()->EndllesInit();

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

	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(true);
	}

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->SetDiceUi();

	// BGMê›íË
	StopSound(SOUND_LABEL_BGM_TITLE);
	PlaySound(SOUND_LABEL_BGM_GAME);
}

void Endless::AfterChange()
{
	DiceManager::GetInstance()->Uninit();
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(false);
	}

	// BGMê›íË
	StopSound(SOUND_LABEL_BGM_GAME);
	PlaySound(SOUND_LABEL_BGM_TITLE);
}
