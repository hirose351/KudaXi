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
	AddGameObject();
}

MaingameScene::~MaingameScene()
{
	Dispose();
}

void MaingameScene::AddGameObject()
{
	sp<Player> player;
	player.SetPtr(new Player);
	mObjList.emplace_back(player);

	sp<Dice> dice;
	dice.SetPtr(new Dice);
	dice->GetTransform()->SetPosition(Float3(0, 8.1f, -DICESCALE));
	mObjList.emplace_back(dice);

	sp<Dice> dice1;
	dice1.SetPtr(new Dice);
	dice1->GetTransform()->SetPosition(Float3(DICESCALE, 8.1f, -DICESCALE * 2));
	mObjList.emplace_back(dice1);

	sp<Dice> dice2;
	dice2.SetPtr(new Dice);
	dice2->GetTransform()->SetPosition(Float3(DICESCALE * 2, 8.1f, -DICESCALE * 2));
	mObjList.emplace_back(dice2);

	sp<Stage> stage;
	stage.SetPtr(new Stage);
	mObjList.emplace_back(stage);
}

bool MaingameScene::Init()
{
	for (auto obj = mObjList.begin(); obj != mObjList.end();)
	{
		(*obj)->Init();
		obj++;
	}
	return true;
}

void MaingameScene::Update()
{
	for (auto obj = mObjList.begin(); obj != mObjList.end();)
	{
		(*obj)->Update();
		//if (!(*obj)->GetLife())
		//{
		//	obj->Clear();
		//	obj = mObjList.erase(obj);
		//	continue;
		//}
		obj++;
	}

	CollisionManager::GetInstance().Update();
}

void MaingameScene::Render()
{
	for (auto obj = mObjList.begin(); obj != mObjList.end();)
	{
		(*obj)->Draw();
		obj++;
	}
}

void MaingameScene::ImguiDebug()
{
}

bool MaingameScene::Dispose()
{
	for (auto obj = mObjList.begin(); obj != mObjList.end();)
	{
		(*obj)->Uninit();
		obj++;
	}

	mObjList.clear();
	return false;
}
