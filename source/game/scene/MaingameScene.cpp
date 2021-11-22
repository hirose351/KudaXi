#include	"MaingameScene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"

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
	mObjList.emplace_back(dice);
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
		(*obj)->Finalize();
		obj++;
	}

	mObjList.clear();
	return false;
}
