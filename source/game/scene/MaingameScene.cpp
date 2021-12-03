#include	"MaingameScene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../manager/collision_manager.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"

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

	Dice* dice = new Dice;
	dice->GetTransform()->SetPosition(Float3(0, 8.1f, -DICESCALE));

	Dice* dice1 = new Dice;
	dice1->GetTransform()->SetPosition(Float3(DICESCALE, 8.1f, -DICESCALE * 2));

	Dice* dice2 = new Dice;
	dice2->GetTransform()->SetPosition(Float3(DICESCALE * 2, 8.1f, -DICESCALE * 2));

	Stage* stage = new Stage;
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
