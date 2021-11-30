#include	"MaingameScene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../manager/collision_manager.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/plane.h"

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
	dice->GetTransform()->SetPosition(Float3(0, 0, 30));
	mObjList.emplace_back(dice);
	sp<Dice> dice1;
	dice1.SetPtr(new Dice);
	dice1->GetTransform()->SetPosition(Float3(30, 0, 0));
	mObjList.emplace_back(dice1);
	sp<Dice> dice2;
	dice2.SetPtr(new Dice);
	dice2->GetTransform()->SetPosition(Float3(-30, 0, 0));
	mObjList.emplace_back(dice2);
	sp<Plane> plane;
	plane.SetPtr(new Plane);
	mObjList.emplace_back(plane);
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
