#include	"maingame_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../gameobject/ui_button.h"
#include	"../gameobject/access_dice_manager.h"
#include	"../../application.h"
#include	"../../system/dx11/DX11util.h"
#include	"../state/gamemode_controller.h"
#include	"../../system/util/XAudio2.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

void MaingameScene::SceneAfter()
{
	//if (mMaingameController.IsExist())
	//	mMaingameController->Init();
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

	Dix::sp<DiceManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceManagerAccess);
	AddGameObject(dicemanager);

	Dix::sp<GameObject> maingameController;
	maingameController.SetPtr(new GameObject(("GameModeController"), ObjectType::eObstracle, false));
	maingameController->AddComponent<Component::GameModeController>()->SetPlayer(player);
	maingameController->GetComponent<Component::GameModeController>()->SetStage(stage);
	AddGameObject(maingameController);
	mMaingameController = maingameController;
}

void MaingameScene::SceneUpdate()
{
}