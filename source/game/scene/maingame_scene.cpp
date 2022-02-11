#include	"maingame_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../state/gamemode_controller.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

void MaingameScene::SceneAfter()
{
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

	Dix::sp<GameObject> maingameController;
	maingameController.SetPtr(new GameObject(("GameModeController"), ObjectType::eObstracle, false));
	maingameController->AddComponent<Component::GameModeController>()->SetPlayer(player);
	maingameController->GetComponent<Component::GameModeController>()->SetStage(stage);
	AddGameObject(maingameController);
}

void MaingameScene::SceneUpdate()
{
}