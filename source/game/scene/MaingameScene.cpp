#include	"MaingameScene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

MaingameScene::~MaingameScene()
{
	Dispose();
}

bool MaingameScene::Init()
{
	sp<Player> player;
	player.SetPtr(new Player);
	mObjList.emplace_back(player);
	sp<Dice> dice;
	dice.SetPtr(new Dice);
	mObjList.emplace_back(dice);

	for (auto item = mObjList.begin(); item != mObjList.end();)
	{
		(*item)->Init();
		item++;
	}
	return true;
}

void MaingameScene::Update()
{
	for (auto item = mObjList.begin(); item != mObjList.end();)
	{
		(*item)->Update();
		//if (!(*item)->GetLife())
		//{
		//	item->Clear();
		//	item = mObjList.erase(item);
		//	continue;
		//}
		item++;
	}
}

void MaingameScene::Render()
{
	for (auto item = mObjList.begin(); item != mObjList.end();)
	{
		(*item)->Draw();
		item++;
	}
}

void MaingameScene::ImguiDebug()
{
}

bool MaingameScene::Dispose()
{
	for (auto item = mObjList.begin(); item != mObjList.end();)
	{
		(*item)->Finalize();
		item++;
	}
	return false;
}
