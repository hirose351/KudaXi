#include	"maingame_scene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../gameobject/ui_button.h"
#include	"../gameobject/pause_endless.h"
#include	"../gameobject/access_dice_manager.h"
#include	"../../application.h"
#include	"../../system/dx11/DX11util.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

void MaingameScene::SceneAfter()
{
	StageDataManager::GetInstance().SetCurrentStage("puzzle/10");
	StageData stageData;
	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	mCameraLookat.x = stageData.mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -stageData.mMapSizeHeight*DICE_SCALE_HALF };
	Camera::GetInstance()->SetEye(Float3(140, 130, -170));
	Camera::GetInstance()->SetLookat(mCameraLookat);
	Camera::GetInstance()->CreateCameraMatrix();
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

	Dix::sp<myUI::PauseEndless> p;
	p.SetPtr(new myUI::PauseEndless);
	AddGameObject(p);

	Dix::sp<AccessDiceManager> dicemanager;
	dicemanager.SetPtr(new AccessDiceManager);
	AddGameObject(dicemanager);

	// �J����
	DirectX::XMFLOAT3 eye(140, 130, -170);	//�J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//�����_
	DirectX::XMFLOAT3 up(0, 1, 0);		//�J�����̏�����x�N�g��
	Camera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);				// �J�����̃f�[�^
}

void MaingameScene::SceneUpdate()
{
}