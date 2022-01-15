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

MaingameScene::~MaingameScene()
{
	Dispose();
}

void MaingameScene::SceneAfter()
{
	StageDataManager::GetInstance().SetCurrentStage("puzzle/10");
	StageData stageData;
	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	mCameraLookat.x = stageData.mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -stageData.mMapSizeHeight*DICE_SCALE_HALF };
	CCamera::GetInstance()->SetEye(Float3(140, 130, -170));
	CCamera::GetInstance()->SetLookat(mCameraLookat);
	CCamera::GetInstance()->CreateCameraMatrix();

}

void MaingameScene::SceneInit()
{
	Player* player = new Player;
	Stage* stage = new Stage;
	Skydome* skydome = new Skydome;

	myUI::PauseEndless* p = new myUI::PauseEndless;

	AccessDiceManager* dicemanager = new AccessDiceManager;

	// カメラ
	DirectX::XMFLOAT3 eye(140, 130, -170);	//カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//注視点
	DirectX::XMFLOAT3 up(0, 1, 0);		//カメラの上向きベクトル
	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);				// カメラのデータ
	SceneAfter();
}

void MaingameScene::SceneUpdate()
{
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

	static DirectX::XMFLOAT4 lightPos = { -50, -87, 66, 0 };

	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 180), ImGuiCond_Once);
	ImGui::Begin(u8"Scene");
	{
		ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat("x", &lightPos.x, 0.5f);
			ImGui::DragFloat("y", &lightPos.y, 0.5f);
			ImGui::DragFloat("z", &lightPos.z, 0.5f);
			DX11LightInit(lightPos);	// 平行光源をセット
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

bool MaingameScene::Dispose()
{
	return false;
}
