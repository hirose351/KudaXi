#pragma once
#include	"scene_base.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	<list>

using Microsoft::WRL::ComPtr;

//// 状態
//enum class GAMESTATE
//{
//	SETUP,	// 準備
//	PLAY,	// プレイ中
//	PAUSE,	// 中断
//	OVER,	// ゲームオーバー
//	CLEAR,	// クリア
//	SELECT,	// セレクト
//};

class MaingameScene : public SceneBase
{
private:
	DirectX::XMFLOAT4 lightPos = { -50, -87, 66, 0 };

public:
	MaingameScene();
	~MaingameScene();

	void SceneInit() override;
	void SceneUpdate() override;
	void SceneRender() override;
	void ImguiDebug() override;
	bool Dispose() override;
};