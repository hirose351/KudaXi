#include	"stagecreate_scene.h"
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

using namespace Dix;

StageCreateScene::StageCreateScene()
{
}

void StageCreateScene::SceneAfter()
{
}

void StageCreateScene::SceneInit()
{
	Dix::sp<Player> player;
	player.SetPtr(new Player);
	AddGameObject(player);

	Dix::sp<Stage> stage;
	stage.SetPtr(new Stage);
	AddGameObject(stage);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	skydome->SetType(Skydome::Type::CLEATE);
	AddGameObject(skydome);

	Dix::sp<DiceManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceManagerAccess);
	AddGameObject(dicemanager);
}

void StageCreateScene::SceneUpdate()
{
}