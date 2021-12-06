#include	"maingame_scene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../manager/collision_manager.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../manager/dice_manager.h"
#include	"../gameobject/skydome.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

MaingameScene::~MaingameScene()
{
	Dispose();
}

void MaingameScene::SceneInit()
{
	Player* player = new Player;
	Stage* stage = new Stage;
	Skydome* skydome = new Skydome;
	DiceManager::GetInstance()->Init();
}

void MaingameScene::SceneUpdate()
{


	CollisionManager::GetInstance().Update();
}

void MaingameScene::SceneRender()
{
}

//void MaingameScene::ImguiDebug()
//{
//}

bool MaingameScene::Dispose()
{
	return false;
}
