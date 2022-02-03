#include	"maingame_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../gameobject/ui_button.h"
#include	"../gameobject/pause_endless.h"
#include	"../gameobject/access_dice_manager.h"
#include	"../../application.h"
#include	"../../system/dx11/DX11util.h"
#include	"../state/gamemode_controller.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

void MaingameScene::SceneAfter()
{
	StageDataManager::GetInstance().SetCurrentStage("puzzle/10");
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();

	mCameraLookat.x = stageData->mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -stageData->mMapSizeHeight*DICE_SCALE_HALF };
	Camera::GetInstance()->SetLookat(mCameraLookat);
	mCameraEye = { 140, 130, -170 };
	Camera::GetInstance()->SetEye(mCameraEye);
	Camera::GetInstance()->CreateCameraMatrix();
}

void MaingameScene::SceneInit()
{
	SceneAfter();
	Dix::sp<Player> player;
	player.SetPtr(new Player);
	AddGameObject(player);

	Dix::sp<Stage> stage;
	stage.SetPtr(new Stage);
	AddGameObject(stage);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	AddGameObject(skydome);

	Dix::sp<myUI::PauseEndless> p;
	p.SetPtr(new myUI::PauseEndless);
	AddGameObject(p);

	Dix::sp<DiceManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceManagerAccess);
	AddGameObject(dicemanager);

	Dix::sp<GameObject> maingameController;
	maingameController.SetPtr(new GameObject(("GameModeController"), ObjectType::eObstracle, false));
	maingameController->AddComponent<Component::GameModeController>()->SetPlayer(player);
	AddGameObject(maingameController);
}

void MaingameScene::SceneUpdate()
{
}