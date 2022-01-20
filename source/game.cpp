#include	"game.h"
#include	"application.h"
#include	"game/manager/scene_manager.h"
#include	"game/scene/title_scene.h"
#include	"game/scene/maingame_scene.h"
#include	"game/scene/modeselect_scene.h"
#include	"game/scene/stagecreate_scene.h"
#include	"game/gameobject/camera.h"
#include	"system/dx11/CDirectInput.h"
#include	"system/dx11/DX11Settransform.h"
#include	"system/imgui/util/myimgui.h"
#include	"system/util/2dsystem.h"
#include	"system/util/XAudio2.h"
#include	"system/util/controller_input.h"

void GameInit() {

	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

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

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(-50, -87, 66, 0));

	// アルファブレンド有効化
	TurnOnAlphablend();

	// 2Dシステム初期化
	Init2D();

	// DirectT INPUT 初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// IMGUI初期化
	imguiInit();

	// 
	InitSound();

	GameSceneInit();
}

void GameSceneInit()
{
	// シーンマネージャにタイトルシーンを登録
	SceneManager::GetInstance()->AddScene<MaingameScene>("GameMain");
	SceneManager::GetInstance()->AddScene<StageCreateScene>("Create");
	SceneManager::GetInstance()->AddScene<ModeSelectScene>("Mode");
	SceneManager::GetInstance()->AddScene<TitleScene>("Title");
	// 現在シーンをTitleにする
	SceneManager::GetInstance()->SetCurrentScene("Title");
}

void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();
	ControllerInput::GetInstance().Input();
}

void UpdateCamera()
{

}

void GameUpdate(uint64_t dt) {
	SceneManager::GetInstance()->Update();
}

void GameRender(uint64_t dt) {

	float col[4] = { 0,0,1,1 };	// 塗りつぶしカラー

	// 描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// プロジェクション変換行列取得
	mtx = Camera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eProjection, mtx);

	// ビュー変換行列を取得
	mtx = Camera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eView, mtx);

	SceneManager::GetInstance()->Render();

	// 描画後処理
	DX11AfterRender();
}

void GameDispose() {
	UninitSound();
	imguiExit();
	DX11Uninit();
}