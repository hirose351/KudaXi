#include	"gamemode_state_endless.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../gameobject/pause_endless.h"
#include	"../component/player_controller.h"

using namespace GameModeState;

Endless::Endless()
{
	Dix::sp<myUI::PauseEndless> p;
	p.SetPtr(new myUI::PauseEndless);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(p);
	p->SetIsActive(false);
	mModeObjList.emplace_back(p);
}

Endless::~Endless()
{
}

void Endless::Exec()
{
	DiceManager::GetInstance()->EndleesUpdate();
	// –ß‚é‚ğ‰Ÿ‚³‚ê‚½‚Æ‚«‚Ìˆ—
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

	Float3 cameraLookat;
	cameraLookat.x = stageData->mMapSizeWidth*DICE_SCALE_HALF;
	cameraLookat.y = { 0 };
	cameraLookat.z = { -stageData->mMapSizeHeight*DICE_SCALE_HALF };
	Camera::GetInstance()->SetLookat(cameraLookat);
	Camera::GetInstance()->SetEye(Float3(140, 130, -170));
	Camera::GetInstance()->CreateCameraMatrix();

	DiceManager::GetInstance()->Init();
	mHolder->GetStage()->ObjectInit();
	mHolder->GetPlayer()->Init();

	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(true);
	}

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->SetDiceUi();
}

void Endless::AfterChange()
{
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(false);
	}
	DiceManager::GetInstance()->Uninit();
}
