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

	// カメラ
	DirectX::XMFLOAT3 eye(140, 130, -170);	//カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//注視点
	DirectX::XMFLOAT3 up(0, 1, 0);		//カメラの上向きベクトル
	Camera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);				// カメラのデータ
}

void StageCreateScene::SceneUpdate()
{
}