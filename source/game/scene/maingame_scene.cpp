#include	"maingame_scene.h"
#include	"../../Memory.h"
#include	"../gameobject/gameobject.h"
#include	"../manager/collision_manager.h"
#include	"../gameobject/player.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../manager/dice_manager.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"

#include	"../../system/dx11/CDirectInput.h"

#include	"../gameobject/ui_button.h"
#include	"../gameobject/pause_endless.h"

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
	// ƒJƒƒ‰•ÏX
	//mCameraLookat.x = StageDataManager::GetInstance().GetCurrentStage()->mMapSizeWidth*DICE_SCALE / 2.0f;
	//mCameraLookat.y = { 0 };
	//mCameraLookat.z = { -StageDataManager::GetInstance().GetCurrentStage()->mMapSizeHeight*DICE_SCALE / 2.0f };


	StageDataManager::GetInstance().SetCurrentStage("puzzle/10");
	StageData stageData;
	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	Player* player = new Player;
	Stage* stage = new Stage;
	Skydome* skydome = new Skydome;

	//myUI::ButtonGroup* bG = new myUI::ButtonGroup;
	//bG->SetInitState("assets/image/ui/number.png", 10, 1, 10, ButtonTransition::eColorTint, XMFLOAT2(100, 300), XMFLOAT2(100, 100), XMFLOAT2(150, 150));

	myUI::PauseEndless* pause = new myUI::PauseEndless;

	mCameraLookat.x = stageData.mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -stageData.mMapSizeHeight*DICE_SCALE_HALF };
	CCamera::GetInstance()->SetLookat(mCameraLookat);
	CCamera::GetInstance()->CreateCameraMatrix();

	DiceManager::GetInstance()->Init();
}

void MaingameScene::SceneUpdate()
{
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		Init();
	}

	CollisionManager::GetInstance().Update();
	DiceManager::GetInstance()->Update();
}

void MaingameScene::SceneRender()
{
}

void MaingameScene::ImguiDebug()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiCond_Once);
	ImGui::Begin(u8"GameObject");
	for (auto &obj : mObjectList)
	{
		obj->ImguiDraw();
	}
	ImGui::End();

	DiceManager::GetInstance()->ImguiDraw();
}

bool MaingameScene::Dispose()
{
	return false;
}
